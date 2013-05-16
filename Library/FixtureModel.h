// -*- mode: C++ -*-
#ifndef _FIXTUREMODEL_H_
#define _FIXTUREMODEL_H_

#include <wx/wx.h>
#include <wx/socket.h>

#include <list>

#include "CITPDefines.h"

class CITP_Fixture;

class CITP_FixtureModel
{


public:
  CITP_FixtureModel();
  virtual ~CITP_FixtureModel();

  // methods required for table model
  //virtual Qt::ItemFlags flags(const QModelIndex &index) const;
  //virtual QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
  //virtual QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;
  //virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
  //virtual int columnCount(const QModelIndex &parent=QModelIndex()) const;

  void addFixture(CITP_Fixture *fix);

  std::list<CITP_Fixture*> m_fixtureList;

};

#endif // _FIXTUREMODEL_H_
