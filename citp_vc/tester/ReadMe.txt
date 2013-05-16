

ok, was also als nächstes:

x die medienserver extensions MSEX
x layer etc simulieren
- befragen, was ist layer was ist videosource 
- prewview simulieren
- einbinden


ok,

mal drüber nachdenken, wie man das am besten
einbindet.

ein songleton würde sich anbieten, oder einfach
ein dreckiger statischer zeiger. ist ja irgendwie
auch das gleiche, ausser das man beim zeiger weiss
ab wann er dirty ist =)

wichtige abfragen:

#define COOKIE_MSEX       0x5845534d // 'MSEX'
#define COOKIE_MSEX_SINF  0x666E6953 // 'SInf'
#define COOKIE_MSEX_GVSR  0x72535647 // 'GVSr' Browser wants video sources

... trivial ...

#define COOKIE_MSEX_GELI  0x494C4547 // 'GELI' Get Element Library Information message

... hier wird ein preload abgefragt ... ?!

#define COOKIE_MSEX_GEIN  0x6E494547 // 'GEIn' Get Element Information message

... thumbs abfragen
stefan: von allen effekten einen preview erstellen

#define COOKIE_MSEX_GELT  0x54454C47 // 'GELT' Get Element Library Thumbnail message

?!

#define COOKIE_MSEX_GETH  0x68544547 // 'GETh' Get Element Thumbnail message

?!

#define COOKIE_MSEX_RQST  0x74537152 // 'RqSt' Request Stream message

layer in echtzeit





Ich glaube, das beste ist, im citp objekt flags zu setzen,
und im allgemeinen rendertraversal abzuhandeln.
variante wäre, ins event system zu gehen aber IO trifft nicht
so richtig und eventhandler auch nicht.

1. bekommt 


















