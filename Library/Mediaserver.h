// -*- mode: C++ -*-
#ifndef _MEDIASERVER_H_
#define _MEDIASERVER_H_


#include "stdafx.h"
#include "CITPDefines.h"
#include "Peer.h"


#using namespace std

class CITP_Mediaserver : public CITP_Peer
{

public:
	CITP_Mediaserver( wxIPaddress &host, uint16 listeningPort, const wxString &peerName, const wxString &peerState );
	virtual ~CITP_Mediaserver();

	
	//
	// video stream requests
	bool isVideoStreamRequested( uint16 sourceidentifier ) { return source_stream_request.find(sourceidentifier) != source_stream_request.end();  }
	bool sendJpegToStream( uint16 sourceidentifier, const char* jpeg_data, long jpeg_buffersize );

	struct VideoSource
	{
		WCHAR name[64];
		char osx_style_lookup_adress[64];
		long Width;
		long Height;
		long Identifier;
	};
	typedef std::vector<VideoSource> t_VideoSourceArray;
	void setAvailableVideoSources( t_VideoSourceArray VideoSourceArray );

private:
	virtual void insertMSEXHeader(unsigned char* byteArray, uint32 msgsize);
	virtual void parseMSEXPacket(const char* byteArray);
	virtual void parseSINFPacket(const char* byteArray);
	virtual void parseGVSRPacket(const char* byteArray);
	virtual void parseRQSTPacket(const char* byteArray);

	t_VideoSourceArray m_VideoSourceArray;

	std::map< uint16, CITP_MSEX_RqSt > source_stream_request;

};

#endif // _MEDIASERVER_H_
