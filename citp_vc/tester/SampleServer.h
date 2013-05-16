// -*- mode: C++ -*-
#ifndef _SAMPLESERVER_H_
#define _SAMPLESERVER_H_


#include "stdafx.h"

DECLARE_EVENT_TYPE(wxEVT_PEER_UPDATED, wxID_ANY)



class CITPLib;
class CITP_Mediaserver;

class SampleServer : public wxEvtHandler
{

public:
  SampleServer();
  virtual ~SampleServer();				    

private:
  void on_actionExit_triggered();
  void on_connectButton_clicked();
  void on_sendNameButton_clicked();
  void on_sendDataButton_clicked();

  void on_sendPatchButton_clicked();
  void on_patchButton_clicked();
  void on_unpatchButton_clicked();

  void on_selectPushButton_clicked();
  void on_deselectPushButton_clicked();

  void handlePeersUpdated();
  void OnHandlePeersUpdated( wxCommandEvent& e );


  void handleUnpatchAllFixtures();
  void handleUnpatchFixures( std::list<uint16> &fixtureIdentifiers);

  void handleSelectFixtures(bool complete, std::list<uint16> &fixtureIdentifiers);

  void handleDeselectAllFixtures();
  void handleDeselectFixtures( std::list<uint16> &fixtureIdentifiers);

  void handleUpdatedFixtureList();

private:

  CITPLib *m_citp;
  CITP_Mediaserver *m_mediaserver;
	
  	//
	// any class wishing to process wxWidgets events must use this macro
	// we want to receive socket events, so declare it
	DECLARE_EVENT_TABLE()


};

#endif // _SAMPLESERVER_H_
