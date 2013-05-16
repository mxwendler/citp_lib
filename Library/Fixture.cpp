
#include "stdafx.h"
#include "Fixture.h"

CITP_Fixture::CITP_Fixture( uint16 fixtureIdentifier, uint8 universe, uint16 channel, uint16 channelCount, 
	  const wxString &fixtureMake, const wxString &fixtureName )
  : 
    m_fixtureIdentifier(fixtureIdentifier),
    m_universe(universe),
    m_channel(channel),
    m_channelCount(channelCount),
    m_fixtureMake(fixtureMake),
    m_fixtureName(fixtureName)
{
}

CITP_Fixture::~CITP_Fixture()
{
}
