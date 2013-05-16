// -*- mode: C++ -*-
#ifndef _PACKETCREATOR_H_
#define _PACKETCREATOR_H_


#include <wx/wx.h>
#include <list>
#include "CITPDefines.h"

using namespace std;

typedef foreach_vector<long> vsu;
typedef foreach_vector<string> vst;
typedef foreach_vector<wstring> vwt;

static size_t buf_creation_calc_packet_length( vsu structs, vst charstrings, vwt wcharstrings )
{
	//
	// vec a) longs, in bytes, add up
	// vec b) strings, count bytes, add \0, mul with sizeof
	// vec b) wstrings, count bytes, add \0, mul with sizeof
	size_t bufsize = 0;
	foreach(structs)
	{
		bufsize += structs.current();
	}
	foreach(charstrings)
	{
		bufsize += (charstrings.current().size()+1) * sizeof(char);
	}
	foreach(wcharstrings)
	{
		bufsize += (wcharstrings.current().size()+1) * sizeof(WCHAR);
	}
	return  bufsize;
}

static void buf_creation_offset_struct_by_string_len( char** orig_struct, std::wstring wcharstring )
{
	//
	// this func takes a stuct pointer and a
	// wchar string. it will move the structs adress
	// so the string fits into the struct and further
	// data can be written.
	size_t strlen = (wcharstring.size()+1) *sizeof(WCHAR);
	size_t strloffset = strlen - sizeof(ucs2*);
	*orig_struct += strloffset;
}

class CITP_PacketCreator
{

public:
  CITP_PacketCreator();
  virtual ~CITP_PacketCreator();

  typedef enum
    {
      LightingServer,
      MediaServer,
      Visualizer,
      OperationHub
    } PLocType;





  // returns NULL on error. bufferLen is the length of the returned buffer.
  static unsigned char * createPLocPacket(PLocType plocType, int listeningPort, const wxString &name, 
					  const wxString &state, int &bufferLen);

  // returns NULL on error. bufferLen is the length of the returned buffer.
  static unsigned char * createUNamPacket( uint8 universeIndex, const wxString &name,
					  int &bufferLen);

  // returns NULL on error. bufferLen is the length of the returned buffer.
  static unsigned char * createChBkPacket(bool blind,
							uint8 universeIndex,
							uint16 firstChannelIndex,
							uint16 channelCount,
							const uint8 *channelLevels,
							int &bufferLen);

  static unsigned char * createSXSrPacket_ArtNet(int network, int universe, int channel, int &bufferLen);
  static unsigned char * createSXSrPacket_Avab(int network, int universe, int channel, int &bufferLen);
  static unsigned char * createSXSrPacket_E131(int universe, int channel, int &bufferLen);
  static unsigned char * createSXSrPacket_ETCNet2(int channel, int &bufferLen);

  static unsigned char * createPtchPacket( uint16 fixtureIdentifier, 
											uint8 universeIndex, 
											uint16 channelIndex,
											uint16 channelCount, 
											const wxString &fixtureMake, 
											const wxString &fixtureName,
											int &bufferLen);

  static unsigned char * createSPtcPacket( std::list<uint16> &fixtureIdentifiers, int &bufferLen);

  static unsigned char * createUPtcPacket( std::list<uint16> &fixtureIdentifiers, int &bufferLen);

  static unsigned char * createSelePacket( std::list<uint16> &fixtureIdentifiers, 
					  bool complete, int &bufferLen);

  static unsigned char * createDeSePacket( std::list<uint16> &fixtureIdentifiers, int &bufferLen);

private:

  static unsigned char *createSXSrPacket(const wxString &str, int &bufferLen);

};

#endif // _PACKETCREATOR_H_
