
#include "stdafx.h"
#include "citp-lib.h"

#include "CITPDefines.h"

#include "Peer.h"
#include "PeerInformationSocket.h"

CITP_MasterNode::CITP_MasterNode()
  :
    m_peerSocket(NULL)
{
  m_peerSocket = new CITP_PeerInformationSocket();
}

CITP_MasterNode::~CITP_MasterNode()
{
	delete m_peerSocket;
}

bool CITP_MasterNode::createPeerInformationSocket( wxString name, wxString state, long port )
{
	if (!m_peerSocket)
	{
		return false;
	}
	return m_peerSocket->init( port, name, state);
}

bool CITP_MasterNode::listPeers(std::list<CITP_Peer*> &peerList)
{
	if (!m_peerSocket)
	{
		return false;
	}
	return m_peerSocket->listPeers(peerList);
}
