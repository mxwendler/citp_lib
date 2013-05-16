

#include "stdafx.h"
#include "CITPDefines.h"

#include "Visualizer.h"


#include "PacketCreator.h"

CITP_Visualizer::CITP_Visualizer( wxIPaddress &host, uint16 listeningPort,
       const wxString &peerName, const wxString &peerState )
  : CITP_Peer( host, listeningPort, peerName, peerState )
{
  wxLogDebug("Creating visualizer peer");
  m_peerType = VISUALIZER_PEER;
}

CITP_Visualizer::~CITP_Visualizer()
{
}

bool CITP_Visualizer::sendUniverseName(uint8 universeIndex, const wxString &universeName)
{
  int bufferLen;
  unsigned char *buffer = CITP_PacketCreator::createUNamPacket(universeIndex, universeName, bufferLen);
  if (!buffer)
    {
      wxLogDebug("createUNamPacket() failed");
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      wxLogDebug("CITP_Visualizer::sendUniverseName failed");
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

bool CITP_Visualizer::sendChannelData(bool blind, uint8 universeIndex, 
				 uint16 firstChannelIndex, uint16 channelCount, 
				 const uint8 *channelLevels) 
{
  int bufferLen;
  unsigned char *buffer = CITP_PacketCreator::createChBkPacket(blind,
							  universeIndex,
							  firstChannelIndex,
							  channelCount,
							  channelLevels,
							  bufferLen);
  if (!buffer)
    {
      wxLogDebug("createChBkPacket() failed");;
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      wxLogDebug("CITP_Visualizer::sendChannelData failed");;
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}


  // Alternate DMX source management
bool CITP_Visualizer::sendAlternateSourceMessage_ArtNet(int network, int universe, int channel)
{
  int bufferLen;
  unsigned char *buffer = CITP_PacketCreator::createSXSrPacket_ArtNet(network, universe, channel, bufferLen);
  if (!buffer)
    {
      wxLogDebug("createSXSrPacket_ArtNet() failed");;
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      wxLogDebug("CITP_Visualizer::sendAlternateSourceMessage_ArtNet failed");;
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}
 
bool CITP_Visualizer::sendAlternateSourceMessage_Avab(int network, int universe, int channel)
{
  int bufferLen;
  unsigned char *buffer = CITP_PacketCreator::createSXSrPacket_Avab(network, universe, channel, bufferLen);
  if (!buffer)
    {
      wxLogDebug("createSXSrPacket_Avab() failed");;
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      wxLogDebug("CITP_Visualizer::sendAlternateSourceMessage_Avab failed");;
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

bool CITP_Visualizer::sendAlternateSourceMessage_E131(int universe, int channel)
{
  int bufferLen;
  unsigned char *buffer = CITP_PacketCreator::createSXSrPacket_E131(universe, channel, bufferLen);
  if (!buffer)
    {
      wxLogDebug("createSXSrPacket_E131() failed");;
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      wxLogDebug("CITP_Visualizer::sendAlternateSourceMessage_E131 failed");;
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

bool CITP_Visualizer::sendAlternateSourceMessage_ETCNet2(int channel)
{
  int bufferLen;
  unsigned char *buffer = CITP_PacketCreator::createSXSrPacket_ETCNet2(channel, bufferLen);
  if (!buffer)
    {
      wxLogDebug("createSXSrPacket_ETCNet2() failed");;
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      wxLogDebug("CITP_Visualizer::sendAlternateSourceMessage_ETCNet2 failed");;
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

bool CITP_Visualizer::sendPatchMessage(uint16 fixtureIdentifier, uint8 universeIndex, uint16 channelIndex,
				  uint16 channelCount, const wxString &fixtureMake, const wxString &fixtureName)
{
  int bufferLen;
  unsigned char *buffer = CITP_PacketCreator::createPtchPacket(fixtureIdentifier,
							  universeIndex,
							  channelIndex,
							  channelCount,
							  fixtureMake,
							  fixtureName,
							  bufferLen);
  if (!buffer)
    {
      wxLogDebug("createPtchPacket() failed");;
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      wxLogDebug("CITP_Visualizer::sendPatchMessage failed");;
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

// send empty list to unpatch all fixtures
bool CITP_Visualizer::sendUnpatchMessage( std::list<uint16> &fixtureIdentifiers)
{
  int bufferLen;
  unsigned char *buffer = CITP_PacketCreator::createUPtcPacket(fixtureIdentifiers, bufferLen);
  if (!buffer)
    {
      wxLogDebug("createUPtcPacket() failed");;
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      wxLogDebug("CITP_Visualizer::sendUnpatchMessage failed");;
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

// send empty list to request entire patch
bool CITP_Visualizer::sendPatchRequest( std::list<uint16> &fixtureIdentifiers)
{
  wxLogDebug("in sendPatchRequest");;

  int bufferLen;
  unsigned char *buffer = CITP_PacketCreator::createSPtcPacket(fixtureIdentifiers, bufferLen);
  if (!buffer)
    {
      wxLogDebug("createSPtcPacket() failed");;
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      wxLogDebug("CITP_Visualizer::sendPatchRequest failed");;
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

bool CITP_Visualizer::sendSelectMessage(bool complete,  std::list<uint16> &fixtureIdentifiers)
{
  int bufferLen;
  unsigned char *buffer = CITP_PacketCreator::createSelePacket(fixtureIdentifiers, complete, bufferLen);
  if (!buffer)
    {
      wxLogDebug("createSelePacket() failed");;
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      wxLogDebug("CITP_Visualizer::sendSelectMessage failed");;
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

bool CITP_Visualizer::sendDeselectMessage( std::list<uint16> &fixtureIdentifiers)
{
  int bufferLen;
  unsigned char *buffer = CITP_PacketCreator::createDeSePacket(fixtureIdentifiers, bufferLen);
  if (!buffer)
    {
      wxLogDebug("createDeSePacket() failed");;
      return false;
    }

  if (!sendPacket(buffer, bufferLen))
    {
      wxLogDebug("CITP_Visualizer::sendDeselectMessage failed");;
      delete[] buffer;
      return false;
    }

  delete[] buffer;
  return true;
}

