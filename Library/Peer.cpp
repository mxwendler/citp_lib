
#include "stdafx.h"
#include "CITPDefines.h"

#include "Peer.h"
#include "PacketCreator.h"
#include "Fixture.h"
#include "FixtureModel.h"



CITP_Peer::CITP_Peer( wxIPaddress &host, uint16 listeningPort,
       const wxString &peerName, const wxString &peerState )
  :
    m_peerType(UNKNOWN_PEER),
    m_peerState(peerState),
    m_peerName(peerName),
    m_host(&host),
    m_listeningPort(listeningPort),
	connection_socket(0)
{
	m_fixtureModel = new CITP_FixtureModel();

	wxIPV4address addr;
	m_tcpSocket = new wxSocketServer(host);
	m_tcpSocket->SetEventHandler( *this );
	m_tcpSocket->SetNotify( wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG );
	m_tcpSocket->Notify( true );
	if(!m_tcpSocket->Ok())
	{
		wxLogDebug("socket creation error");
	}
}

CITP_Peer::~CITP_Peer()
{
  if (m_tcpSocket)
    {
      m_tcpSocket->Close();
    }
}

void CITP_Peer::OnSocket( wxSocketEvent& e )
{
    switch(e.GetSocketEvent())
    {
		case wxSOCKET_CONNECTION:
        {
			wxLogDebug("connecting");
			connection_socket = m_tcpSocket->Accept(true);
			connection_socket->SetEventHandler( *this );
			connection_socket->SetNotify( wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG );
			connection_socket->Notify( true );
            break;
        }
        case wxSOCKET_INPUT:
        {
			wxLogDebug("input");
			handleReadyRead();
            break;
        }
        case wxSOCKET_LOST:
        {
			wxLogDebug("lost");
            break;
        }
        default: ;
    }
}

CITP_PeerType CITP_Peer::peerType() const
{
  return m_peerType;
}

wxString CITP_Peer::peerName() const
{
  return m_peerName;
}

wxString CITP_Peer::peerState() const
{
  return m_peerState;
}

wxString CITP_Peer::peerHost() const
{
  return m_host->IPAddress();
}

uint16 CITP_Peer::peerListeningPort() const
{
  return m_listeningPort;
}

bool CITP_Peer::connectToPeer()
{
  if (0 == m_listeningPort)
    {
      wxLogDebug("CITP_Peer::connect() - CITP_Peer is not listening for connections");
      return false;
    }

  if (!m_tcpSocket)
    {
      return false;
    }
  
  //m_tcpSocket->connectToHost(m_host, m_listeningPort);

  // XXX - use waitForConnected() ?

  return true;
}

bool CITP_Peer::sendPacket( unsigned char *buffer, int bufferLen)
{
	connection_socket->Write( buffer, bufferLen);
	return true;

	wxLogDebug("hw not implemented");
	return false;
}

void CITP_Peer::handleReadyRead()
{
	const long peekSize = 12;
	char peekArray[peekSize];

	connection_socket->Peek( peekArray, peekSize );
	if(connection_socket->LastCount()!=peekSize)
	{
		return;
	}

	if (
		peekArray[0] != 'C' && peekArray[1] != 'I' &&
		peekArray[2] != 'T' && peekArray[3] != 'P')
	{
		return;
	}

	uint32 messageSize = 0;
	messageSize = peekArray[11];
	messageSize <<= 8;
	messageSize |= peekArray[10];
	messageSize <<= 8;
	messageSize |= peekArray[9];
	messageSize <<= 8;
	messageSize |= peekArray[8];

	const long bufsize = 10*1024;
	char buffer[bufsize];
	connection_socket->Read( buffer, messageSize );
	if(connection_socket->LastCount()!= messageSize )
	{
		return;
	}
	parsePacket(buffer);
}

void CITP_Peer::parsePacket(const char* byteArray)
{
  const char *data = byteArray;
  struct CITP_Header *citpHeader = (struct CITP_Header*)data;

  // CITP header
  if (citpHeader->Cookie != COOKIE_CITP)
    {
      wxLogDebug("parsePacket: invalid Cookie");// << citpHeader->Cookie;
      return;
    }

  if (citpHeader->VersionMajor != 0x01)
    {
      wxLogDebug("parsePacket: invalid VersionMajor:");//<< citpHeader->VersionMajor;
      return;
    }

  // XXX - this coming back as 0x01 during patch operations?
  /*
  if (citpHeader->VersionMinor != 0x00)
    {
      qDebug() << "parsePacket: invalid VersionMinor:" << citpHeader->VersionMinor;
      return;
    }
  */
  //packet->bufferLen;
  //packet->MessagePartCount = 0x01;
  //packet->MessagePart = 0x01; // XXX - doc says 0-based?
  
  switch (citpHeader->ContentType)
    {
    case COOKIE_PINF:
      wxLogDebug("parsePacket: got COOKIE_PINF packet on TCP Socket...");//"parsePacket: got COOKIE_PINF packet on TCP Socket...";
      return;
      break;

    case COOKIE_FPTC:
      parseFPTCPacket(byteArray);
      break;

    case COOKIE_FSEL:
      parseFSELPacket(byteArray);
      break;

    case COOKIE_MSEX:
      parseMSEXPacket(byteArray);
      break;

    default:
      wxLogDebug("parsePacket: unknown ContentType:" );
      return;
    }
}

void CITP_Peer::parseFPTCPacket(const char* byteArray)
{
  const char *data = byteArray;
  struct CITP_FPTC_Header *fptcHeader = (struct CITP_FPTC_Header*)data;

  switch (fptcHeader->ContentType)
    {
    case COOKIE_FPTC_PTCH:
      parsePTCHPacket(byteArray);
      break;

    case COOKIE_FPTC_UPTC:
      parseUPTCPacket(byteArray);
      break;

    case COOKIE_FPTC_SPTC:
      parseSPTCPacket(byteArray);
      break;

    default:
      wxLogDebug("parseFPTCPacket: unknown ContentType:");//  << fptcHeader->ContentType;
    }
}

void CITP_Peer::parseUPTCPacket(const char* byteArray)
{
  const char *data = byteArray;
  struct CITP_FPTC_UPtc *uptcPacket = (struct CITP_FPTC_UPtc*)data;

  uint16 fixtureCount = uptcPacket->FixtureCount;
  if (0 == fixtureCount)
    {
      //emit unpatchAllFixtures();
      return;
    }

  std::list<uint16> fixtureIdentifiers;
  int offset = sizeof(struct CITP_FPTC_UPtc);
  for (int i=0; i<fixtureCount; ++i)
    {
      uint16 *fixId = (uint16*)(data + offset);
      fixtureIdentifiers.push_back(*fixId);
      offset += 2;
    }

  wxLogDebug("parseUPTCPacket: Unpatch Fixtures:");;
  //foreach (uint16 id, fixtureIdentifiers)
  //  {
  //    qDebug() << "\tunpatch id:" << id;
  //  }

  //emit unpatchFixtures(fixtureIdentifiers);
}

void CITP_Peer::parseSPTCPacket(const char* byteArray)
{
  const char *data = byteArray;
  struct CITP_FPTC_SPtc *sptcPacket = (struct CITP_FPTC_SPtc*)data;

  uint16 fixtureCount = sptcPacket->FixtureCount;
  if (0 == fixtureCount)
    {
      //emit sendPatchAllFixtures();
      return;
    }

  std::list<uint16> fixtureIdentifiers;
  int offset = sizeof(struct CITP_FPTC_SPtc);
  for (int i=0; i<fixtureCount; ++i)
    {
      uint16 *fixId = (uint16*)(data + offset);
      fixtureIdentifiers.push_back(*fixId);
      offset += 2;
    }

  //qDebug() << "parseSPTCPacket: Unpatch Fixtures:";
  //foreach (uint16 id, fixtureIdentifiers)
  //  {
  //    qDebug() << "\tpatch id:" << id;
  //  }

  //emit sendPatchFixtures(fixtureIdentifiers);
}

void CITP_Peer::parsePTCHPacket(const char* byteArray)
{
  const char *data = byteArray;
  struct CITP_FPTC_Ptch *ptchPacket = (struct CITP_FPTC_Ptch*)data;

  uint16 fixId = ptchPacket->FixtureIdentifier;
  uint8 universe = ptchPacket->Universe;
  uint16 channel = ptchPacket->Channel;
  uint16 channelCount = ptchPacket->ChannelCount;
  
  int offset = sizeof(struct CITP_FPTC_Ptch);
  wxString makeString(data+offset);  

  offset += makeString.size() + 1;
  wxString nameString(data+offset);

  CITP_Fixture *fix = new CITP_Fixture(fixId, universe, channel, channelCount, makeString, nameString );
  //wx_CHECK_PTR(fix);

  //qDebug() << "parseFPTCPacket Id:" << fixId << "universe:" << universe << "channel:" << 
  //  channel << "channelcount:" << channelCount << makeString << nameString;

  m_fixtureList.push_back(fix);
  m_fixtureModel->addFixture(fix);

  //emit updatedFixtureList();
}

void CITP_Peer::parseFSELPacket(const char* byteArray)
{
  const char *data = byteArray;
  struct CITP_FSEL_Header *fselHeader = (struct CITP_FSEL_Header*)data;

  switch (fselHeader->ContentType)
    {
    case COOKIE_FSEL_SELE:
      parseSELEPacket(byteArray);
      break;

    case COOKIE_FSEL_DESE:
      parseDESEPacket(byteArray);
      break;

    default:
      wxLogDebug("parseFSELPacket: unknown ContentType:"); //<<  << fselHeader->ContentType;
    }
}

void CITP_Peer::parseSELEPacket(const char* byteArray)
{
  const char *data = byteArray;
  struct CITP_FSEL_Sele *selePacket = (struct CITP_FSEL_Sele*)data;

  uint16 fixtureCount = selePacket->FixtureCount;
  bool complete = selePacket->Complete;

  std::list<uint16> fixtureIdentifiers;
  int offset = sizeof(struct CITP_FSEL_Sele);
  for (int i=0; i<fixtureCount; ++i)
    {
      uint16 *fixId = (uint16*)(data + offset);
      fixtureIdentifiers.push_back(*fixId);
      offset += 2;
    }

  //qDebug() << "parseSELEPacket: Select Fixtures:";
  //foreach (uint16 id, fixtureIdentifiers)
  //  {
  //    qDebug() << "\tselect id:" << id;
  //  }

  //emit selectFixtures(complete, fixtureIdentifiers);
}

void CITP_Peer::parseDESEPacket(const char* byteArray)
{
  const char *data = byteArray;
  struct CITP_FSEL_DeSe *selePacket = (struct CITP_FSEL_DeSe*)data;

  uint16 fixtureCount = selePacket->FixtureCount;
  if (0 == fixtureCount)
    {
      //emit deselectAllFixtures();
    }

  std::list<uint16> fixtureIdentifiers;
  int offset = sizeof(struct CITP_FSEL_DeSe);
  for (int i=0; i<fixtureCount; ++i)
    {
      uint16 *fixId = (uint16*)(data + offset);
      fixtureIdentifiers.push_back(*fixId);
      offset += 2;
    }
}

BEGIN_EVENT_TABLE( CITP_Peer, wxEvtHandler)
	EVT_SOCKET( wxID_ANY, CITP_Peer::OnSocket )
END_EVENT_TABLE()

