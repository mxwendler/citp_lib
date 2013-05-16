// -*- mode: C++ -*-
#ifndef _CITPDEFINES_H_
#define _CITPDEFINES_H_

//
// The CITP data structures and constants
//

//
// Datatypes
//

//typedef qint8   int8;
//typedef qint16  int16;
//typedef qint32  int32;
//typedef qint64  int64;
//
//typedef quint8  uint8;
//typedef quint16 uint16;
//typedef quint32 uint32;
//typedef quint64 unit64;

typedef unsigned char	Uint8;
typedef signed char	Sint8;
typedef unsigned short	Uint16;
typedef short	Sint16;
typedef unsigned int	Uint32;
typedef int	Sint32;
#ifndef _MSC_VER
	// Probably gcc or something compatible
	typedef unsigned long long	Uint64;
	typedef long long Sint64;
#else	// _MSC_VER
	typedef unsigned __int64	Uint64;
	typedef __int64	Sint64;
#endif // _MSC_VER


typedef Uint8 uint8;
typedef Sint8 sint8;
typedef Sint8 int8;
typedef Uint16 uint16;
typedef Sint16 sint16;
typedef Sint16 int16;
typedef Uint32 uint32;
typedef Sint32 sint32;
typedef Sint32 int32;
typedef Uint64 uint64;
typedef Sint64 sint64;
typedef Sint64 int64;


typedef unsigned char ucs1;

#ifndef WCHAR
	typedef wchar_t WCHAR;
#endif //WCHAR
typedef WCHAR ucs2;


// 
// Content Type cookies
//

//#define COOKIE_CITP       0x43495450    // 'CITP'
#define COOKIE_CITP       0x50544943    // 'CITP'

//#define COOKIE_PINF       0x50494e46    // 'PINF'
#define COOKIE_PINF       0x464e4950    // 'PINF'
#define COOKIE_PINF_PNAM  0x504e616d    // 'PNam'
#define COOKIE_PINF_PLOC  0x636f4c50    // 'PLoc'


#define COOKIE_SDMX       0x584d4453    // 'SDMX'
#define COOKIE_SDMX_ENID  0x456e4964    // 'EnId'
#define COOKIE_SDMX_UNAM  0x6d614e55    // 'UNam'
#define COOKIE_SDMX_CHBK  0x6b426843    // 'ChBk'
#define COOKIE_SDMX_SXSR 0x72535853     // 'SXSr'

//#define COOKIE_FPTC  0x46505443 // 'FTPC'
#define COOKIE_FPTC  0x43545046 // 'FPTC'
#define COOKIE_FPTC_PTCH 0x68637450 // 'Ptch'
#define COOKIE_FPTC_UPTC 0x63745055 // 'UPtc'
#define COOKIE_FPTC_SPTC 0x63745053 // 'SPtc'

//#define COOKIE_FSEL 0x4653454c // 'FSEL'
#define COOKIE_FSEL 0x4c455346 // 'FSEL'
#define COOKIE_FSEL_SELE 0x656c6553 // 'Sele'
#define COOKIE_FSEL_DESE 0x65536544 // 'DeSe'

#define COOKIE_FINF       0x464e4946 // 'FINF'
#define COOKIE_FINF_SFRA  0x61724653 // 'SFra'
#define COOKIE_FINF_FRAM  0x6d617246 // 'Fram'
#define COOKIE_FINF_LSTA  0x6174534c // 'LSta'

#define COOKIE_MSEX       0x5845534d // 'MSEX'
#define COOKIE_MSEX_SINF  0x666E6953 // 'SInf'
#define COOKIE_MSEX_GVSR  0x72535647 // 'GVSr' Browser wants video sources
#define COOKIE_MSEX_VSRC  0x63725356 // 'VSrc' Mediaserver sends video sources
#define COOKIE_MSEX_LSTA  0x6174534c // 'LSta' Layer Status message
#define COOKIE_MSEX_GELI  0x494C4547 // 'GELI' Get Element Library Information message
#define COOKIE_MSEX_ELIN  0x6E494C45 // 'ELIn' Element Library Information message
#define COOKIE_MSEX_ELUP  0x70554C45 // 'ELUp' Element Library Updated message
#define COOKIE_MSEX_GEIN  0x6E494547 // 'GEIn' Get Element Information message
#define COOKIE_MSEX_MEIn  0x6E49454D // 'MEIn' Media Element Information message
#define COOKIE_MSEX_EEIn  0x6E494545 // 'EEIn' Effect Element Information message
#define COOKIE_MSEX_GLEI  0x49454C47 // 'GLEI' Generic Element Information message
#define COOKIE_MSEX_GELT  0x54454C47 // 'GELT' Get Element Library Thumbnail message
#define COOKIE_MSEX_ELTH  0x68544C45 // 'ELTh' Element Library Thumbnail message
#define COOKIE_MSEX_GETH  0x68544547 // 'GETh' Get Element Thumbnail message
#define COOKIE_MSEX_ETHN  0x6E685445 // 'EThn' Element Thumbnail message
#define COOKIE_MSEX_RQST  0x74537152 // 'RqSt' Request Stream message
#define COOKIE_MSEX_STFR  0x72467453 // 'StFr' Stream Frame message

#pragma pack(1)

//
// CITP, Base Layer
//
struct CITP_Header 
{ 
  uint32      Cookie;                        // Set to "CITP".      
  uint8       VersionMajor;                  // Set to 1. 
  uint8       VersionMinor;                  // Set to 0.       
  uint8       Reserved[2];                   // 4-byte alignment       
  uint32      MessageSize;                   // The size of the entire message, including this header.      
  uint16      MessagePartCount;              // Number of message fragments.      
  uint16      MessagePart;                   // Index of this message fragment (0-based).      
  uint32      ContentType;                   // Cookie identifying the type of contents (the name of the second layer). 
}; 


//
// CITP/PINF, Peer Information Layer
//

#define CITP_PINF_MULTICAST_IP   "224.0.0.180"
#define CITP_PINF_MULTICAST_PORT 4809

struct CITP_PINF_Header 
{ 
  CITP_Header CITPHeader;    // The CITP header. CITP ContentType is "PINF". 
  uint32      ContentType;   // A cookie defining which PINF message it is. 
}; 

// DEPRECATED, USE PLoc instead
// Peer Name message
struct CITP_PINF_PNam 
{ 
  CITP_PINF_Header  CITPPINFHeader;   // The CITP PINF header. PINF ContentType is "PNam".          
  //ucs1              Name[];           // The display name of the peer (null terminated). 
                                      // This could be anything from a 
				      // user defined alias for the peer of the name of the 
                                      // product, or a combination. 
}; 

// Peer Location message
struct CITP_PINF_PLoc 
{ 
  CITP_PINF_Header  CITPPINFHeader;     // The CITP PINF header. PINF ContentType is "PLoc". 
  uint16            ListeningTCPPort;   // The port on which the peer is listening for 
                                        // incoming TCP connections. 0 if not listening. 
  //ucs1              Type[];             // Can be "LightingServer", "MediaServer", 
                                        // "Visualizer" or "OperationHub". 
  //ucs1              Name[];             // The display name of the peer. Corresponds to the 
                                        //     PINF/PNam/Name field. 
  //ucs1              State[];            // The display state of the peer. This can be any 
                                        //     descriptive string presentable to the user such 
                                        //     as "Idle", "Running" etc. 
};


//
// CITP, SDMX - Send DMX Layer
//

struct CITP_SDMX_Header 
{ 
  CITP_Header CITPHeader;    // CITP header. CITP ContentType is "SDMX". 
  uint32      ContentType;   // Cookie defining which SDMX message it is. 
}; 

// Encryption Identifier message
struct CITP_SDMX_EnId 
{ 
  CITP_SDMX_Header  CITPSDMXHeader;    // CITP SDMX header. SDMX ContentType is "EnId". 
  //ucs1              Identifier[];      // Encryption scheme identifier. 
}; 

// Universe Name message
struct CITP_SDMX_UNam 
{ 
  CITP_SDMX_Header  CITPSDMXHeader;    // CITP SDMX header. SDMX ContentType is "UNam". 
  uint8             UniverseIndex;     // 0-based index of the universe. 
  //ucs1              UniverseName[];    // Name of the universe. 
}; 

// Channel Block message
struct CITP_SDMX_ChBk 
{ 
  CITP_SDMX_Header  CITPSDMXHeader;    // CITP SDMX header. SDMX ContentType is "ChBk". 
  uint8             Blind;             // Set to 1 for blind preview dmx, 0 otherwise. 
  uint8             UniverseIndex;     // 0-based index of the universe. 
  uint16            FirstChannel;      // 0-based index of first channel in the universe. 
  uint16            ChannelCount;      // Number of channels. 
  //uint8             ChannelLevels[];   // Raw channel levels. 
}; 


// DMX Connection Strings

// ArtNet "ArtNet/<net>/<universe>/<channel>" 
// "ArtNet/0/0/1" - The first channel of the first universe on the first network. 
#define ARTNET_STRING tr("ArtNet/%1/%2/%3")

// Avab IPX "AvabIPX/<net>/<universe>/<channel>" 
// "AvabIPX/0/0/1" - The first channel of the first universe on the first network. 
#define AVAB_STRING tr("AvabIPX/%1/%2/%3")

// BSR E1.31 "BSRE1.31/<universe>/<channel>" 
// "BSRE1.31/0/1" - The first channel of the first universe. 
#define E131_STRING tr("BSRE1.31/%1/%2")

// ETC Net2 "ETCNet2/<channel>" 
// "ETCNet2/1" - The first ETCNet2 channel.
#define ETCNET2_STRING tr("ETCNet2/%1")

// Set External Source message
struct CITP_SDMX_SXSr
{ 
  CITP_SDMX_Header  CITPSDMXHeader;       // CITP SDMX header. SDMX ContentType is "SXSr". 
  //ucs1              ConnectionString[];   // DMX-source connection string. See DMX 
                                          // Connection Strings in Definitions. 
};


//
// CITP, FPTC - Fixture Patch Layer
//

struct CITP_FPTC_Header 
{ 
  CITP_Header CITPHeader;    // The CITP header. CITP ContentType is "FPTC". 
  uint32      ContentType;   // A cookie defining which FPTC message it is. 
  uint32      ContentHint;   // Content hint flags. 
                             // 0x00000001    Message part of a sequence of messages. 
                             // 0x00000002    Message part of and ends a sequence of messages. 
}; 

// Fixture Patch message
struct CITP_FPTC_Ptch 
{ 
  CITP_FPTC_Header  CITPFPTCHeader;      // The CITP FPTC header. FPTC ContentType is "Ptch". 
  uint16            FixtureIdentifier;   // Fixture identifier. 
  uint8             Universe;            // Patch universe (0-based). 
  uint8             Reserved[1];         // 4-byte alignment. 
  uint16            Channel;             // Patch channel (0-based). 
  uint16            ChannelCount;        // Patch channel count (1-512). 
  //ucs1              FixtureMake[];       // Fixture make (only null if omitted). 
  //ucs1              FixtureName[];       // Fixture name (never omitted). 
}; 

// Fixture Unpatch message
struct CITP_FPTC_UPtc 
{ 
  CITP_FPTC_Header  CITPFPTCHeader;          // The CITP FPTC header. FPTC ContentType is "UPtc". 
  uint16            FixtureCount;            // Fixture count (0 to unpatch all). 
  //uint16            FixtureIdentifiers[];    // Fixture identifiers 
}; 

// Fixture Send Patch message
struct CITP_FPTC_SPtc 
{ 
  CITP_FPTC_Header  CITPFPTCHeader;          // The CITP FPTC header. FPTC ContentType is "SPtc". 
  uint16            FixtureCount;            // Fixture count (0 to request all). 
  //uint16            FixtureIdentifiers[];    // Fixture identifiers. 
};


//
// CITP, FSEL - Fixture Selection Layer
//

struct CITP_FSEL_Header 
{ 
  CITP_Header CITPHeader;    // The CITP header. CITP ContentType is "FSEL". 
  uint32      ContentType;   // A cookie defining which FSEL message it is. 
}; 

// Fixture Select message
struct CITP_FSEL_Sele 
{ 
  CITP_FSEL_Header  CITPFSELHeader;          // The CITP FSEL header. FSEL ContentType is  "Sele".                  
  uint8             Complete;                // Set to non-zero for complete selection              
  uint8             Reserved[1];             // 4-byte alignment                            
  uint16            FixtureCount;            // Greater than 0                           
  //uint16            FixtureIdentifiers[];    // Fixture identifiers 
};

// Fixture Deselect message
struct CITP_FSEL_DeSe 
{ 
  CITP_FSEL_Header  CITPFSELHeader;          // The CITP FSEL header. FSEL ContentType is  "DeSe".                  
  uint16            FixtureCount;            // 0 for complete deselection                          
  //uint16            FixtureIdentifiers[];    // Fixture identifiers 
};

struct CITP_MSEX_Header
{
    CITP_Header        CITPHeader;            // CITP header. CITP ContentType is "MSEX". 
    uint8              VersionMajor;          // See below.       
    uint8              VersionMinor;          // See below.       
    uint32             ContentType;           // Cookie defining which MSEX message it is.
};

//Currently acknowledged versions of MSEX are 1.0 and 1.1. During a session messages of varying MSEX versions
//may be sent and received. See the MSEX / SInf message also regarding supported version signalling.
//10.2 Message definitions: General media server information
//10.2.1 MSEX / SInf - Server Information message
//The ServerInformation provides the receiver with product and layer information. The version field of the MSEX
//header should be set to the highest supported version of MSEX messages.

struct CITP_MSEX_SInf
{
    CITP_MSEX_Header   CITPMSEXHeader;        // CITP MSEX header. MSEX ContentType is "SInf". See above on version. 
    ucs2*              ProductName;         // Display name of the product.                         
    uint8              ProductVersionMajor;   // Major version number of the product.                 
    uint8              ProductVersionMinor;   // Minor version number of the product.                 
    uint8              LayerCount;            // Number of following layer information blocks.        
    struct LayerInformation
    {
        ucs1*           DMXSource;           // DMX-source connection string. See DMX Connection Strings in Definitions.
    };
};

//MSEX / GVSr - GetVideoSources
//The GetVideoSources message is sent to a media 
//server in order to receive all available video source feeds.
struct CITP_MSEX_GVSr
{
    CITP_MSEX_Header   CITPMSEXHeader;        // CITP MSEX header. MSEX ContentType is "GVSr".
};

//10.5.2 MSEX / VSrc - Video Sources
//The VideoSources message is sent in response to a GetVideoSources message. 
//The PhysicalOutput and LayerNumber fields can be used for automatic connection
//to outputs and individual layers (for instance the video of
//output 1 would have PhysicalOutput = 0 and LayerNumber = 0xFF).
struct CITP_MSEX_VSrc
{
    CITP_MSEX_Header   CITPMSEXHeader;        // CITP MSEX header. MSEX ContentType is "VSrc".
    uint16             SourceCount;           // Number of following source information blocks.
};

struct SourceInformation
{
    uint16         SourceIdentifier;    // Source identifier.
    ucs2*			SourceName;		// Display name of the source (ie "Output 1", "Layer 2", "Camera 1" etc).
    uint8          PhysicalOutput;      // If applicable, 0-based index designating the physical video output index. Otherwise 0xFF.
    uint8          LayerNumber;         // If applicable, 0-based layer number, corresponding to the layers reported in the SInf message. Otherwise 0xFF.
    uint16         Flags;               // Information flags.
                                        // 0x0001 Without effects
    uint16         Width;               // Full width.
    uint16         Height;              // Full height.
};


//10.5.3 MSEX / RqSt - Request Stream message
//The RequestStream message is sent by a Server or visualizer to a media server in order to create a time limited
//subscription of a video source. The media server will not provide multiple resolutions and frame rates of a single
//source, but it may provide a feed for each requested format. It is up to the peer to regularly request a stream,
//based on its timeout parameter, if it wishes receive a continuous feed. High values of the timeout field is of course
//discouraged.
struct CITP_MSEX_RqSt
{
    CITP_MSEX_Header   CITPMSEXHeader;        // CITP MSEX header. MSEX ContentType is "RqSt".
    uint16             SourceIdentifier;      // Identifier of the source requested.
    uint32             FrameFormat;           // Requested frame format. Can be "RGB8" or "JPEG".
    uint16             FrameWidth;            // Preferred minimum frame width.
    uint16             FrameHeight;           // Preferred minimum frame height.
    uint8              FPS;                   // Preferred minimum frames per second.
    uint8              Timeout;               // Timeout in seconds (for instance 5 seconds, 0 to ask for only one frame).
};

//10.5.4 MSEX / StFr - Stream Frame message
//The StreamFrame message is multicasted regularly from a media server. 
//The resolutions, formats and FPS are
//determine by the current set of subscribing peers.

#define FRAMEFORMATJPEG 0x4745504A
#define FRAMEFORMATRGB8 0x38424752

struct CITP_MSEX_StFr
{
    CITP_MSEX_Header   CITPMSEXHeader;        // The CITP MSEX header. MSEX ContentType is "StFr".
    uint16             SourceIdentifier;      // Identifier of the frame's source.
    uint32             FrameFormat;           // Requested frame format. Can be "RGB8" or "JPEG".
    uint16             FrameWidth;            // Preferred minimum frame width.
    uint16             FrameHeight;           // Preferred minimum frame height.
    uint16             FrameBufferSize;       // Size of the frame image buffer.
    uint8*             FrameBuffer;         // Frame image buffer.
};

//10.2.2 MSEX / LSta - Layer Status message
//The LayerStatus message is sent at a regular interval (suggestion: 4 times / second) to provide the receiver with
//live status information:
struct CITP_MSEX_LSta
{
    CITP_MSEX_Header   CITPMSEXHeader;        // CITP MSEX header. MSEX ContentType is "LSta" and version is 1.0.      
    uint8              LayerCount;            // Number of following layer information blocks.
    struct LayerStatus
    {
        uint8          LayerNumber;           // 0-based layer number, corresponding to the layers reported in the SInf message.
        uint8          PhysicalOutput;        // Current physical video output index, 0-based.
        uint8          MediaLibraryNumber;    // Current media library number.   
        uint8          MediaNumber;           // Current media number.   
        ucs2*          MediaName;           // Current media name.   
        uint32         MediaPosition;         // Current media position (in frames).   
        uint32         MediaLength;           // Current media length (in frames).   
        uint8          MediaFPS;              // Current media resolution in frames per second.   
        uint32         LayerStatusFlags;      // Current layer status flags
                                              //      0x0001 MediaPlaying
    };
	LayerStatus*	   LayerStatus;
};

#endif // _CITPDEFINES_H_
