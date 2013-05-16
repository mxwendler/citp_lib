// -*- mode: C++ -*-
#ifndef _PEERINFORMATIONSOCKET_H_
#define _PEERINFORMATIONSOCKET_H_

#include "CITPDefines.h"


class wxTimer;
class CITP_Peer;

struct CITP_PeerDescripton;

class CITP_PeerInformationSocket 
	:
	public wxEvtHandler,
	public wxThread
{

public:
	CITP_PeerInformationSocket();
	virtual ~CITP_PeerInformationSocket();

	bool init(const int listeningPort, const wxString &name, const wxString &state);
	bool listPeers(std::list<CITP_Peer*> &peerList);

	static bool multicast( const void* buf, wxUint32 nBytes );


private:
	void transmitPLoc();
	void handleReadReady();

	void* Entry( void );

	static wxDatagramSocket* m_socket;

	wxString m_name;
	wxString m_state;

	unsigned char *m_packetBuffer;
	int m_packetBufferLen;

	void processPacket( wxIPaddress &sender, const char* packet);

	void addPeer( wxIPaddress &host, uint16 listeningPort, const wxString &typeString, const wxString &name, const wxString &state);

	std::list<CITP_Peer*> m_peerList;

	DECLARE_EVENT_TABLE()

};

#endif // _PEERINFORMATIONSOCKET_H_
