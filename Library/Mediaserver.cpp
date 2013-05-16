

#include "stdafx.h"
#include "CITPDefines.h"
#include "Mediaserver.h"
#include "PacketCreator.h"
#include "PeerInformationSocket.h"

using namespace std;

CITP_Mediaserver::CITP_Mediaserver( wxIPaddress &host, uint16 listeningPort,
       const wxString &peerName, const wxString &peerState )
  : CITP_Peer( host, listeningPort, peerName, peerState )
{
  m_peerType = MEDIASERVER_PEER;
}

CITP_Mediaserver::~CITP_Mediaserver()
{
}

void CITP_Mediaserver::parseMSEXPacket(const char* byteArray)
{
  const char *data = byteArray;
  struct CITP_MSEX_Header *msexHeader = (struct CITP_MSEX_Header*)data;

  switch (msexHeader->ContentType)
    {
    case COOKIE_MSEX_SINF:
      parseSINFPacket(byteArray);
      break;
    case COOKIE_MSEX_GVSR:
      parseGVSRPacket(byteArray);
      break;
	case COOKIE_MSEX_LSTA  : { wxLogDebug("LSta - Layer Status message"); }  break; // 'LSta'
	case COOKIE_MSEX_GELI  : { wxLogDebug("Get Element Library Information message"); }  break; // 'GELI'
	case COOKIE_MSEX_ELIN  : { wxLogDebug("Element Library Information message"); }  break; // 'ELIn'
	case COOKIE_MSEX_ELUP  : { wxLogDebug("Element Library Updated message"); }  break; // 'ELUp'
	case COOKIE_MSEX_GEIN  : { wxLogDebug("Get Element Information message"); }  break; // 'GEIn'
	case COOKIE_MSEX_MEIn  : { wxLogDebug("Media Element Information message"); }  break; // 'MEIn'
	case COOKIE_MSEX_EEIn  : { wxLogDebug("Effect Element Information message"); }  break; // 'EEIn'
	case COOKIE_MSEX_GLEI  : { wxLogDebug("Generic Element Information message"); }  break; // 'GLEI'
	case COOKIE_MSEX_GELT  : { wxLogDebug("Get Element Library Thumbnail message"); }  break; // 'GELT'
	case COOKIE_MSEX_ELTH  : { wxLogDebug("Element Library Thumbnail message"); }  break; // 'ELTh'
	case COOKIE_MSEX_GETH  : { wxLogDebug("Get Element Thumbnail message"); }  break; // 'GETh'
	case COOKIE_MSEX_ETHN  : { wxLogDebug("Element Thumbnail message"); }  break; // 'EThn'
	case COOKIE_MSEX_RQST  : { wxLogDebug("Request Stream message"); parseRQSTPacket(byteArray); }  break; // 'RqSt'
	case COOKIE_MSEX_STFR  : { wxLogDebug("Stream Frame message"); }  break; // 'StFr'
    default:
      wxLogDebug("parseMSEXPacket: unknown ContentType");
    }
}

void CITP_Mediaserver::insertMSEXHeader(unsigned char* byteArray, uint32 msgsize)
{
	// 
	// print in a CITP MSEX header
	// casting to CITP_MSEX_StFr here but it could be anything
	CITP_MSEX_StFr* v = ( CITP_MSEX_StFr* ) byteArray;
	v->CITPMSEXHeader.VersionMajor = 0x01;
	v->CITPMSEXHeader.VersionMinor = 0x00;
	v->CITPMSEXHeader.CITPHeader.Cookie = COOKIE_CITP;
	v->CITPMSEXHeader.CITPHeader.VersionMajor = 0x01;
	v->CITPMSEXHeader.CITPHeader.VersionMinor = 0x00;
	v->CITPMSEXHeader.CITPHeader.Reserved[0] = 0x00;
	v->CITPMSEXHeader.CITPHeader.Reserved[1] = 0x00; 
	v->CITPMSEXHeader.CITPHeader.MessageSize = msgsize;
	v->CITPMSEXHeader.CITPHeader.MessagePartCount = 0x01;
	v->CITPMSEXHeader.CITPHeader.MessagePart = 0x00;
	v->CITPMSEXHeader.CITPHeader.ContentType = COOKIE_MSEX;
}

void CITP_Mediaserver::parseSINFPacket(const char* byteArray)
{
  const char *data = byteArray;
  struct CITP_MSEX_SInf *sinfPacket = (struct CITP_MSEX_SInf*)data;
}

void CITP_Mediaserver::parseRQSTPacket(const char* byteArray)
{

	//
	// handle async: store request, serve later
	// ( on frame end )
	const char *data = byteArray;
	struct CITP_MSEX_RqSt *rq = (struct CITP_MSEX_RqSt*)data;
	source_stream_request[ rq->SourceIdentifier ] = *rq;

}

void CITP_Mediaserver::setAvailableVideoSources( t_VideoSourceArray _VideoSourceArray )
{
	m_VideoSourceArray = _VideoSourceArray;
}

bool CITP_Mediaserver::sendJpegToStream( uint16 sourceidentifier, const char* jpeg_data, long jpeg_buffersize )
{

	// sane request?
	if( jpeg_buffersize==0 )
	{ 
		return false; 
	}

	// sane request?
	if( jpeg_data==0 )
	{ 
		return false; 
	}

	// request avilable?
	if( source_stream_request.find(sourceidentifier) == source_stream_request.end())
	{ 
		return false; 
	}
	
	//
	// send a frame
	CITP_MSEX_RqSt rq = source_stream_request[sourceidentifier];
	if( rq.FrameFormat==FRAMEFORMATJPEG )
	{
		size_t stfr = sizeof(CITP_MSEX_StFr) - sizeof(uint8*);
		size_t msgsize = jpeg_buffersize + stfr;

		unsigned char* buffer = new unsigned char[ msgsize ];
		memset( buffer, 0, msgsize );

		//
		// build StFr CITP header
		CITP_MSEX_StFr* v = ( CITP_MSEX_StFr* ) buffer;
		insertMSEXHeader( buffer, msgsize );
		v->CITPMSEXHeader.ContentType = COOKIE_MSEX_STFR;

		v->SourceIdentifier = sourceidentifier;
		v->FrameFormat = FRAMEFORMATJPEG;
		v->FrameWidth = rq.FrameWidth;
		v->FrameHeight = rq.FrameHeight;
		v->FrameBufferSize = jpeg_buffersize;
		memcpy( &v->FrameBuffer, jpeg_data, jpeg_buffersize );

		if (!CITP_PeerInformationSocket::multicast( buffer, msgsize ) )
		{
			wxLogDebug("CITP_Mediaserver::COOKIE_MSEX_STFR failed");
			return false;
		}

		delete [] buffer;		
	}	

	return true;
}

void CITP_Mediaserver::parseGVSRPacket(const char* byteArray)
{

	//
	// there is not so much to parse here.
	// answer with a CITP_MSEX_VSrc message
	size_t videosourcecount = m_VideoSourceArray.size();

	//
	// must need to know future packet struct size. 
	// 1. add main struct.
	vsu stru;
	vst stri;
	vwt wstri;
	stru.push_back(sizeof(CITP_MSEX_VSrc));

	//
	// now iterate sources
	for( size_t i=0; i<videosourcecount; i++ )
	{
		// add video source struct size.
		stru.push_back(sizeof(SourceInformation));

		// add video source name ( wchar! )
		WCHAR* name = m_VideoSourceArray[i].name;
		wstri.push_back( name );
	}

	//
	// get packet size, create struct, insert CITP MSEX header
	size_t packetsize = buf_creation_calc_packet_length(stru, stri, wstri);
	unsigned char* buffer = new unsigned char[ packetsize ];
	memset( buffer, 0, packetsize );
	CITP_MSEX_VSrc* v = ( CITP_MSEX_VSrc* ) buffer;
	insertMSEXHeader( buffer, packetsize );

	//
	// insert data
	v->CITPMSEXHeader.ContentType = COOKIE_MSEX_VSRC;
	v->SourceCount = videosourcecount;

	//
	// iterate sources
	SourceInformation* s = (SourceInformation*) ( buffer + sizeof(CITP_MSEX_VSrc) );
	for( size_t i=0; i<videosourcecount; i++ )
	{
		s->SourceIdentifier = m_VideoSourceArray[i].Identifier;

		WCHAR* source_name = m_VideoSourceArray[i].name;
		memcpy( &(s->SourceName), source_name, wcslen(source_name)*sizeof(WCHAR) );
		buf_creation_offset_struct_by_string_len( (char**) &s, source_name );

		s->PhysicalOutput = 0xFF;
		s->LayerNumber = 0xFF;
		s->Flags = 0x0001;
		s->Width = m_VideoSourceArray[i].Width;
		s->Height = m_VideoSourceArray[i].Height;

		// shift pointer
		s += 1;
	}

 	if (!sendPacket( buffer, packetsize ) )
	{
		wxLogDebug("CITP_Mediaserver::parseGVSRPacket failed");;
		return;
	}

	delete [] buffer;
}
