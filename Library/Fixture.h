// -*- mode: C++ -*-
#ifndef _FIXTURE_H_
#define _FIXTURE_H_

#include <wx/wx.h>
#include "CITPDefines.h"

class CITP_Fixture
{

public:
  CITP_Fixture( uint16 fixtureIdentifier, uint8 universe, uint16 channel, uint16 channelCount, 
	  const wxString &fixtureMake, const wxString &fixtureName );
  virtual ~CITP_Fixture();


  uint16 m_fixtureIdentifier;
  uint8 m_universe;
  uint16 m_channel;
  uint16 m_channelCount;
  wxString m_fixtureMake;
  wxString m_fixtureName;

};

#endif // _FIXTURE_H_

