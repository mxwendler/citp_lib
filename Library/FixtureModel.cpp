
#include "stdafx.h"
#include "CITPDefines.h"

#include "FixtureModel.h"
#include "Fixture.h"

#define NUM_COLS 6
// id universe channel channelcount make name

CITP_FixtureModel::CITP_FixtureModel()
{
}

CITP_FixtureModel::~CITP_FixtureModel()
{
}

void CITP_FixtureModel::addFixture(CITP_Fixture *fix)
{
  m_fixtureList.push_back(fix);
}
