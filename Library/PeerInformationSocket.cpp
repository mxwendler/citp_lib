
#include "stdafx.h"

#include "citp-lib.h"
#include "CITPDefines.h"
#include "PacketCreator.h"
#include "Peer.h"
#include "Visualizer.h"
#include "PeerInformationSocket.h"

#define TRANSMIT_INTERVAL_MS  1000
#define TIMER_ID 24

//
// application wide socket for multicasting
// used for browsing, video streams etc.
wxDatagramSocket* CITP_PeerInformationSocket::m_socket = 0;

void* CITP_PeerInformationSocket::Entry( void )
{
	long interval = TRANSMIT_INTERVAL_MS / 10;
	while(!TestDestroy())
	{
		Sleep( interval );	
		static long runtime = 0;
		runtime += interval;
		if(runtime % TRANSMIT_INTERVAL_MS == 0)
		{
			transmitPLoc();
		}
	}
	return 0;
}

CITP_PeerInformationSocket::CITP_PeerInformationSocket()
  : 
    m_name("Unknown name"),
    m_state("Unknown state"),
    m_packetBuffer(NULL),
    m_packetBufferLen(0),
	wxThread( wxTHREAD_JOINABLE )
{
	Create();
	Run();
}

CITP_PeerInformationSocket::~CITP_PeerInformationSocket()
{
	Delete(); // the thread: stop it continue freeing up resources
	delete m_packetBuffer;
	delete m_socket;
}

bool CITP_PeerInformationSocket::init(const int listeningPort, const wxString &name, const wxString &state)
{
	m_name = name;
	m_state = state;

	wxIPV4address receive_adress;
	receive_adress.AnyAddress();
	receive_adress.Service( CITP_PINF_MULTICAST_PORT );
	m_socket = new wxDatagramSocket( receive_adress, wxSOCKET_NOWAIT );

	m_socket->SetFlags ( wxSOCKET_REUSEADDR );
	m_socket->SetLocal ( receive_adress );
	m_socket->SetNotify(wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	m_socket->Notify(true);

	m_packetBuffer = CITP_PacketCreator::createPLocPacket( CITP_PacketCreator::MediaServer, listeningPort, name, state, m_packetBufferLen);
	if (!m_packetBuffer)
	{
		m_packetBufferLen = 0;
		return false;
	}

	transmitPLoc();  
	return true;
}

bool CITP_PeerInformationSocket::multicast( const void* buf, wxUint32 nBytes )
{
	wxIPV4address addr;
	addr.Hostname(CITP_PINF_MULTICAST_IP);
	addr.Service(CITP_PINF_MULTICAST_PORT);
	m_socket->SendTo( addr, buf, nBytes );
	if (m_socket->Error())
	{
		wxLogDebug("Failed to send multicast packet:");
		return false;
	}

	return true;
}

void CITP_PeerInformationSocket::transmitPLoc()
{
	if (m_packetBuffer && m_packetBufferLen > 0)
	{
		multicast( m_packetBuffer, m_packetBufferLen );
	}
}

void CITP_PeerInformationSocket::handleReadReady()
{
	//while (hasPendingDatagrams()) 
	//{
	//wxByteArray datagram;
	//datagram.resize(pendingDatagramSize());
	//wxHostAddress sender;
	//uint16 senderPort;

	//if (-1 != readDatagram(datagram.data(), datagram.size(),
	//&sender, &senderPort))
	//{
	//processPacket(sender, datagram);
	//}
	//}
}

void CITP_PeerInformationSocket::processPacket( wxIPaddress &address, const char* packetArray)
{
  const char *data = packetArray;
  CITP_PINF_PLoc *packet = (CITP_PINF_PLoc*)data;

  // CITP header
  if (packet->CITPPINFHeader.CITPHeader.Cookie != COOKIE_CITP)
    {
      return;
    }

  if (packet->CITPPINFHeader.CITPHeader.VersionMajor != 0x01)
    {
      wxLogDebug("Invalid VersionMajor value:"); //<< packet->CITPPINFHeader.CITPHeader.VersionMajor;
      return;
    }

  if (packet->CITPPINFHeader.CITPHeader.VersionMinor != 0x00)
    {
      wxLogDebug("Invalid VersionMinor value:");// << packet->CITPPINFHeader.CITPHeader.VersionMinor;
      return;
    }
  //packet->CITPPINFHeader.CITPHeader.Reserved[0] = 0x00;
  //packet->CITPPINFHeader.CITPHeader.Reserved[1] = 0x00; 
  //packet->CITPPINFHeader.CITPHeader.MessageSize = bufferLen;
  //packet->CITPPINFHeader.CITPHeader.MessagePartCount = 0x01;
  //packet->CITPPINFHeader.CITPHeader.MessagePart = 0x01; // XXX - doc says 0-based?
  
  if (packet->CITPPINFHeader.CITPHeader.ContentType != COOKIE_PINF)
    {
      return;
    }

  // PINF header
  if (packet->CITPPINFHeader.ContentType != COOKIE_PINF_PLOC)
    {
      return;
    }

  // PLoc data
  uint16 listeningPort = packet->ListeningTCPPort;
  if (0 == listeningPort)
    {
      // XXX - uncomment if we don't want non-listeners
      //return;
    }
  
  // type
  int offset = sizeof(struct CITP_PINF_PLoc);
  wxString typeString(packetArray +offset);
  //memcpy(buffer + offset, typeString.toAscii().constData(), typeString.size());
  
  // name
  offset += typeString.size() + 1;
  wxString nameString(packetArray +offset);
  //memcpy(buffer + offset, name.toAscii().constData(), name.size());

  // state
  offset += nameString.size() + 1;
  wxString stateString(packetArray +offset);
  //memcpy(buffer + offset, state.toAscii().constData(), state.size());

  // XXX - these strings are going out of scope because they are implicitly shared
  // from the original buffer

  addPeer(address, listeningPort, typeString, nameString, stateString);
}

void CITP_PeerInformationSocket::addPeer( wxIPaddress &host, uint16 listeningPort, 
				    const wxString &typeString, 
				    const wxString &name, const wxString &state)
{
  // check if we already have this peer
	//foreach (const Peer *peer, m_peerList)
	//{
	//	if (peer)
	//	{
	//		if (peer->peerHost() == host.toString() && peer->peerListeningPort() == listeningPort)
	//		{
	//			return;
	//		}
	//	}
	//}

  //  qDebug() << "Adding new peer to list:" << host.toString() << listeningPort << typeString << name << state;

  // add the newly discovered peer
  CITP_Peer *newPeer;
  if ("Visualizer" == typeString)
    {
      newPeer = new CITP_Visualizer(host, listeningPort, name, state);
    }
  else
    {
      newPeer = new CITP_Peer(host, listeningPort, name, state);
    }

  m_peerList.push_back(newPeer);

  wxLogDebug("added peer, peer list size:");// << m_peerList.size();

  //emit peersUpdated();
}

bool CITP_PeerInformationSocket::listPeers(std::list<CITP_Peer*> &peerList)
{
  peerList = m_peerList;
  return true;
}

BEGIN_EVENT_TABLE(CITP_PeerInformationSocket, wxEvtHandler)
END_EVENT_TABLE()
