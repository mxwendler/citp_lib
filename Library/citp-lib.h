// -*- mode: C++ -*-
#ifndef _CITPLIB_H_
#define _CITPLIB_H_

#include <wx/wx.h>
#include <list>


class CITP_PeerInformationSocket;
class CITP_Peer;

class CITP_MasterNode 
	: 
	public wxEvtHandler
{
	public:
		CITP_MasterNode();
		virtual ~CITP_MasterNode();

		bool createPeerInformationSocket( wxString name, wxString state, long port );
		bool listPeers(std::list<CITP_Peer*> &peerList);

	private:
		CITP_PeerInformationSocket *m_peerSocket;
		void peersUpdated();
};

#endif // _CITPLIB_H_
