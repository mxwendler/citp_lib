// -*- mode: C++ -*-
#ifndef _PEER_H_
#define _PEER_H_

#include <wx/wx.h>
#include <wx/socket.h>

#include <list>

#include "CITPDefines.h"


class CITP_Fixture;
class CITP_FixtureModel;

typedef enum
  {
    UNKNOWN_PEER,
    VISUALIZER_PEER,
    MEDIASERVER_PEER,
    LIGHTINGServer_PEER,
    OPERATIONSHUB_PEER,
  } CITP_PeerType;

class CITP_Peer
	:
	public wxEvtHandler
{

public:
	CITP_Peer( wxIPaddress &host, uint16 listeningPort, const wxString &peerName, const wxString &peerState );
  virtual ~CITP_Peer();

  
  virtual wxString peerName() const;
  virtual wxString peerState() const;
  virtual wxString peerHost() const;
  virtual uint16 peerListeningPort() const;

  virtual CITP_PeerType peerType() const;

  virtual bool connectToPeer();

  CITP_FixtureModel *m_fixtureModel;
  std::list<CITP_Fixture*> m_fixtureList;

 protected:

	CITP_PeerType m_peerType;

	wxString m_peerState;
	wxString m_peerName;

	wxIPaddress* m_host;
	uint16 m_listeningPort;

	wxSocketServer *m_tcpSocket;
	wxSocketBase* connection_socket;

	bool sendPacket( unsigned char *buffer, int bufferLen);

	void parsePacket(const char* byteArray);

	void parseFPTCPacket(const char* byteArray);
	void parseUPTCPacket(const char* byteArray);  
	void parseSPTCPacket(const char* byteArray);
	void parsePTCHPacket(const char* byteArray);

	void parseFSELPacket(const char* byteArray);
	void parseSELEPacket(const char* byteArray);
	void parseDESEPacket(const char* byteArray);

	virtual void parseMSEXPacket(const char* byteArray) {} // overridden by Mediaserver

protected:

	void OnSocket( wxSocketEvent& e );	

	void handleReadyRead();

	void connectedToPeer();
	void disconnectedFromPeer();

	void unpatchAllFixtures();
	void unpatchFixtures(const std::list<uint16> &fixtureIdentifiers);

	void sendPatchAllFixtures();
	void sendPatchFixtures(const std::list<uint16> &fixtureIdentifiers);

	void selectFixtures(bool complete, const std::list<uint16> &fixtureIdentifiers);

	void deselectFixtures(const std::list<uint16> &fixtureIdentifiers);
	void deselectAllFixtures();

	void updatedFixtureList();

private:

	DECLARE_EVENT_TABLE()

};

#endif _PEER_H_
