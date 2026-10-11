#pragma once
#include "tinyxml2/tinyxml2.h"
#include "cocos2d.h"
#include "LegacyData.h"
#include "MyUtils/CMD5Checksum.h"
#include "MyUtils/sqlite3.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;

class CMD5Checksum;
class SelectLayer;
class NetworkLayer;
class KTools : public Ref{
public:
	KTools(void);
	~KTools(void);

	bool init();
	CREATE_FUNC(KTools);

	void prepareFileMD5();
	void prepareFileSHA();

	bool readXMLToArray(const char* filePath,NarutoSenkiLegacy::Array* &array);
	void	initTableInDB();
	void	initColumeInDB();

	sqlite3* prepareTableInDB();


	
	bool checkData();
	void updateData();
	bool	saveToSQLite(const char* table="GameRecord",const char* column=NULL,const char* value=NULL,bool isBuy=false);
	std::string readFromSQLite(const char* table="GameRecord",const char* column=NULL,const char* value=NULL);
	
	void encode(std::string &str,int randomKey);
	void decode(std::string &str);
	int checkMD5(std::string findPath ="");
	

	void dfsFolder(std::string folderPath, int depth = 0,int type=0);

	bool checkPackage();
	static void preloadEffectCached(const char* soundPath);
	static void unloadEffectCached(const char* soundPath);
	static void preloadMatchEffectsFromXML(const char* xmlPath);
	static void preloadMatchEffect(const char* soundPath);
	static void releaseMatchEffects();
	static unsigned int playEffectOptimized(const char* soundPath, unsigned int minFrameGap=4, int maxPerFrame=5);

	std::string getKeycode(std::string path);
	

	std::string readSQLite(const char* table,const char* column,const char* value,const char* targetColumn);
	void saveSQLite(const char* table,const char* relatedColumn,const char* value,const char* targetColumn,char* targetValue,bool isPlus);


	CC_SYNTHESIZE(SelectLayer*,_delegate,Delegate);
	CC_SYNTHESIZE(NetworkLayer*,_delegate2,Delegate2);


	std::string encodeData(std::string data);
};

class CCTips : public Sprite{
public:
	CCTips(void);
	~CCTips(void);

	virtual bool			init(const char *tips);
	static CCTips*	create(const char *tips);


protected:

	void onDestroy();


};



