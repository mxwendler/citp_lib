// -*- mode: C++ -*-
#ifndef _VISUALIZER_H_
#define _VISUALIZER_H_

#include "Peer.h"

#include <wx/wx.h>
#include <wx/socket.h>

#include <list>

#include "CITPDefines.h"

class CITP_Visualizer : public CITP_Peer
{

public:
	CITP_Visualizer( wxIPaddress &host, uint16 listeningPort, 
	     const wxString &peerName, const wxString &peerState );
  virtual ~CITP_Visualizer();

  bool sendUniverseName(uint8 universeIndex, const wxString &universeName);

  bool sendChannelData(bool blind, uint8 universeIndex, 
		       uint16 firstChannelIndex, uint16 channelCount, 
		       const uint8 *channelLevels);

  bool sendPatchMessage(uint16 fixtureIdentifier, uint8 universeIndex, uint16 channelIndex,
			uint16 channelCount, const wxString &fixtureMake, const wxString &fixtureName);

  // send empty list to unpatch all fixtures
  bool sendUnpatchMessage( std::list<uint16> &fixtureIdentifiers);

  // send empty list to request entire patch
  bool sendPatchRequest( std::list<uint16> &fixtureIdentifiers);

  bool sendSelectMessage(bool complete,  std::list<uint16> &fixtureIdentifiers);
  bool sendDeselectMessage( std::list<uint16> &fixtureIdentifiers);

  // Alternate DMX source management
  bool sendAlternateSourceMessage_ArtNet(int network, int universe, int channel);
  bool sendAlternateSourceMessage_Avab(int network, int universe, int channel);
  bool sendAlternateSourceMessage_E131(int universe, int channel);
  bool sendAlternateSourceMessage_ETCNet2(int channel);


};

#endif // _VISUALIZER_H_
