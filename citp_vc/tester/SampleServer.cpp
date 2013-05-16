
#include "stdafx.h"
#include "SampleServer.h"

//#include "citp-lib.h"
#include "../../Library/Mediaserver.h"
//#include "FixtureModel.h"

DEFINE_EVENT_TYPE(wxEVT_PEER_UPDATED)


SampleServer::SampleServer()
{

	wxSocketBase::Initialize();

	CITP_MasterNode* m_citp = new CITP_MasterNode();
	// connect(m_citp, SIGNAL(peersUpdated()), this, SLOT(handlePeersUpdated()));
	m_citp->Connect( wxID_ANY, wxEVT_PEER_UPDATED, wxCommandEventHandler(SampleServer::OnHandlePeersUpdated) );

	wxIPV4address mediaserver_address;
	mediaserver_address.AnyAddress();
	mediaserver_address.Service( 60000 );
	m_mediaserver = new CITP_Mediaserver( mediaserver_address, mediaserver_address.Service(), "mxw", "xxx" );

	if( !m_citp->createPeerInformationSocket( "MXWendler Media Server", "Running", 60000 ) )
	{
		wxLogDebug( "CreatePeerInformationSocket failed" );
	}


}

SampleServer::~SampleServer()
{
}

void SampleServer::on_actionExit_triggered()
{
  //close();
}

void SampleServer::OnHandlePeersUpdated( wxCommandEvent& e )
{
	
}

void SampleServer::handlePeersUpdated()
{
 // qDebug() << "handlePeersUpdated()";

 // QList<Peer*> peerList;
 // if (m_citp)
 //   {
 //     if (!m_citp->listPeers(peerList))
	//{
	//  qDebug() << "listPeers() failed";
	//  return;
	//}
 //   }

 // ui.textEdit->appendPlainText("\n\nPeer List Updated:");
 // foreach (const Peer *peer, peerList)
 //   {
 //     if (!peer)
	//{
	//  continue;
	//}

 //     QString desc = tr("\t%1, %2 (%3:%4)\n").arg(peer->peerName()).arg(peer->peerState())
	//.arg(peer->peerHost()).arg(peer->peerListeningPort());
 //     ui.textEdit->appendPlainText(desc);

 //   }

}

void SampleServer::on_connectButton_clicked()
{
  //QList<Peer*> peers;
  //m_citp->listPeers(peers);
  //if (2 != peers.size())
  //  {
  //    qDebug() << "not two peers";
  //    return;
  //  }

  //m_visualizer = (Visualizer*)(peers.at(1));
  //if (!m_visualizer->connectToPeer())
  //  {
  //    qDebug() << "ConnectToPeer failed";
  //    return;
  //  }

  //connect(m_visualizer, SIGNAL(unpatchAllFixtures()),
	 // this, SLOT(handleUnpatchAllFixtures()));
  //connect(m_visualizer, SIGNAL(unpatchFixtures(const QList<quint16> &)),
	 // this, SLOT(handleUnpatchFixtures(const QList<quint16> &)));
  //connect(m_visualizer, SIGNAL(updatedFixtureList()),
	 // this, SLOT(handleUpdatedFixtureList()));

  //ui.tableView->setModel(m_visualizer->m_fixtureModel);
}

void SampleServer::on_sendNameButton_clicked()
{
 // if (m_visualizer)
 //   {
 //     if (!m_visualizer->sendUniverseName(0, "universe 0"))
	//{
	//  qDebug() << "send universe name failed";
	//  return;
	//}
 //   }
}

void SampleServer::on_sendDataButton_clicked()
{
 // static quint8 count = 128;

 // if (m_visualizer)
 //   {
 //     bool blind = false;
 //     quint8 universeIndex = 0;
 //     quint16 firstChannelIndex = 0;
 //     quint16 channelCount = 24;
 //     quint8 *channelLevels = new quint8[channelCount];
 //     memset(channelLevels, count, channelCount);

 //     if (!m_visualizer->sendChannelData(blind, universeIndex, firstChannelIndex, 
	//				 channelCount, channelLevels))
	//{
	//  qDebug() << "send channel data failed";
	//}

 //     delete[] channelLevels;
 //     count++;
 //   }
}

void SampleServer::on_sendPatchButton_clicked()
{
 // if (m_visualizer)
 //   {
 //     QList<quint16> fixList;
 //     if (!m_visualizer->sendPatchRequest(fixList))
	//{
	//  qDebug() << "send patch request failed";
	//  return;
	//}
 //   }
}

void SampleServer::on_patchButton_clicked()
{
}

void SampleServer::on_unpatchButton_clicked()
{
 // if (m_visualizer)
 //   {
 //     QList<quint16> fixList;
 //     if (!m_visualizer->sendUnpatchMessage(fixList))
	//{
	//  qDebug() << "send unpatch message failed";
	//  return;
	//}
 //   }
}

void SampleServer::on_selectPushButton_clicked()
{
 // if (m_visualizer)
 //   {
 //     int id = ui.idSpinBox->value();
 //     bool complete = Qt::Checked == ui.completeCheckBox->checkState();
 //     QList<quint16> fixIds;
 //     fixIds.append(id);
 //     if (!m_visualizer->sendSelectMessage(complete, fixIds))
	//{
	//  qDebug() << "send select message failed";
	//  return;
	//}
 //   }
}

void SampleServer::on_deselectPushButton_clicked()
{
 // if (m_visualizer)
 //   {
 //     int id = ui.idSpinBox->value();
 //     QList<quint16> fixIds;
 //     fixIds.append(id);
 //     if (!m_visualizer->sendDeselectMessage(fixIds))
	//{
	//  qDebug() << "send deselect message failed";
	//  return;
	//}
 //   }
}

void SampleServer::handleUnpatchAllFixtures()
{
  // ui.textEdit->appendPlainText(tr("Got Unpatch All Fixtures signal"));
}

void SampleServer::handleUnpatchFixures( std::list<uint16> &fixtureIdentifiers)
{
  // ui.textEdit->appendPlainText(tr("Got Unpatch Fixture List signal"));
}

void SampleServer::handleUpdatedFixtureList()
{
  // ui.textEdit->appendPlainText(tr("Got Updated Fixture List signal"));
}

void SampleServer::handleSelectFixtures(bool complete, std::list<uint16> &fixtureIdentifiers)
{
  // ui.textEdit->appendPlainText(tr("Got Select Fixtures signal"));
}

void SampleServer::handleDeselectAllFixtures()
{
  // ui.textEdit->appendPlainText(tr("Got Deselect All Fixtures signal"));
}

void SampleServer::handleDeselectFixtures( std::list<uint16> &fixtureIdentifiers)
{
  // ui.textEdit->appendPlainText(tr("Got Deselect Fixtures signal"));
}

BEGIN_EVENT_TABLE(SampleServer, wxEvtHandler)
	EVT_COMMAND( wxID_ANY, wxID_ANY, SampleServer::OnHandlePeersUpdated)
END_EVENT_TABLE()