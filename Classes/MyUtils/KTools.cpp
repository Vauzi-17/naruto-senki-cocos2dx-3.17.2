#include "KTools.h"
#include <cstdlib>
#include "SelectLayer.h"
#include "NetworkLayer.h"
/*#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif  */
#include <map>
#include <vector>
#include <string>
#include <algorithm>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

using namespace tinyxml2;
using namespace CocosDenshion;

KTools::KTools(void)
{
	_delegate=NULL;
	_delegate2=NULL;
}


KTools::~KTools(void)
{
}

bool KTools::init(){
	bool bRet=false;
	do{

		bRet=true;

	}while(0);
	
	return bRet;
}
 


void  KTools::prepareFileMD5(){

	char* fileList[]={
		"Element/Bullet/Amaterasu.xml",
		"Element/Bullet/FlyKnife.xml",
		"Element/Bullet/HiraishinKunai.xml",
		"Element/Bullet/HugeSRK.xml",
		"Element/Bullet/Kusuri.xml",
		"Element/Bullet/PaperSpear.xml",
		"Element/Bullet/PaperSrk.xml",
		"Element/Bullet/Shintenshin.xml",
		"Element/Bullet/TentenSRK.xml",

		"Element/Monster/Bikyu.xml",
		"Element/Monster/Bug.xml",
		"Element/Monster/BugPillar.xml",
		"Element/Monster/BugTomado.xml",
		"Element/Monster/ChuiDi.xml",
		"Element/Monster/CircleMark.xml",
		"Element/Monster/ClayBird.xml",
		"Element/Monster/Crash.xml",
		"Element/Monster/Crash2.xml",
		"Element/Monster/DeidaraBom.xml",
		"Element/Monster/Dogs.xml",
		"Element/Monster/FakeDeidara.xml",
		"Element/Monster/FakeItachi.xml",
		"Element/Monster/FakeMinato.xml",
		"Element/Monster/FakeShino.xml",
		"Element/Monster/FakeTobirama.xml",
		"Element/Monster/FireRain.xml",
		"Element/Monster/FudonSRK.xml",
		"Element/Monster/FudonSRK2.xml",
		"Element/Monster/Hasan.xml",
		"Element/Monster/HiraishinMark.xml",
		"Element/Monster/InkBird.xml",
		"Element/Monster/InkDragon.xml",
		"Element/Monster/ItachiSusano.xml",
		"Element/Monster/Kage.xml",
		"Element/Monster/KageBom.xml",
		"Element/Monster/KageFeng.xml",
		"Element/Monster/KageHand.xml",
		"Element/Monster/KageHands.xml",
		"Element/Monster/Kaiten.xml",
		"Element/Monster/Kubi.xml",
		"Element/Monster/Kuroari.xml",
		"Element/Monster/LeeBom.xml",
		"Element/Monster/Mine.xml",
		"Element/Monster/Mouse.xml",
		"Element/Monster/Paku.xml",
		"Element/Monster/PaperRain.xml",
		"Element/Monster/Qilin.xml",
		"Element/Monster/QuanRen.xml",
		"Element/Monster/Sabaku.xml",
		"Element/Monster/SakuraBom.xml",
		"Element/Monster/Sanbao.xml",
		"Element/Monster/SandBall.xml",
		"Element/Monster/SandHand.xml",
		"Element/Monster/SandWave.xml",
		"Element/Monster/SasukeSusano.xml",
		"Element/Monster/Shark.xml",
		"Element/Monster/Shark2.xml",
		"Element/Monster/Shenwei.xml",
		"Element/Monster/Shenwei2.xml",
		"Element/Monster/Shoryu.xml",
		"Element/Monster/SmallSlug.xml",
		"Element/Monster/Snake.xml",
		"Element/Monster/Spider.xml",
		"Element/Monster/Steam.xml",
		"Element/Monster/Suiji.xml",
		"Element/Monster/Suijin.xml",
		"Element/Monster/SuiRyuDan.xml",
		"Element/Monster/TamaBomb.xml",
		"Element/Monster/Tenmu.xml",
		"Element/Monster/ThunderWave.xml",
		"Element/Monster/Tiger.xml",
		"Element/Monster/TodonPillar.xml",
		"Element/Monster/Traps.xml",
		"Element/Monster/Tsukuyomi.xml",
		"Element/Monster/Tuji.xml",
		"Element/Monster/Tuji2.xml",
		"Element/Monster/WaterBom.xml",
		"Element/Monster/WaterBullet.xml",
		"Element/Monster/Wave.xml",
		"Element/Monster/Yataikuzu.xml",
		"Element/Monster/Yominuma.xml",

		"Element/Flog/ObitoFlog.xml",
		"Element/Flog/FemalePainFlog.xml",
		"Element/Flog/IzumoFlog.xml",
		"Element/Flog/KakashiFlog.xml",
		"Element/Flog/KotetsuFlog.xml",
		"Element/Flog/PainFlog.xml",
		"Element/Tower/CenterDate.xml",
		"Element/Tower/TowerDate.xml",

		"Element/Akamaru/Akamaru.xml",
		"Element/Asuma/Asuma.xml",
		"Element/Centipede/Centipede.xml",
		"Element/Chiyo/Chiyo.xml",
		"Element/Choji/Choji.xml",
		"Element/Deidara/Deidara.xml",
		"Element/DogWall/DogWall.xml",
		"Element/Gaara/Gaara.xml",
		"Element/Han/Han.xml",
		"Element/Hidan/Hidan.xml",
		"Element/Hinata/Hinata.xml",
		"Element/Hiruzen/Hiruzen.xml",
		"Element/ImmortalSasuke/ImmortalSasuke.xml",
		"Element/Ino/Ino.xml",
		"Element/Itachi/Itachi.xml",
		"Element/Jiraiya/Jiraiya.xml",
		"Element/Jugo/Jugo.xml",
		"Element/Kakashi/Kakashi.xml",
		"Element/Kakuzu/Kakuzu.xml",
		"Element/Kankuro/Kankuro.xml",
		"Element/Karasu/Karasu.xml",
		"Element/Karin/Karin.xml",
		"Element/Kiba/Kiba.xml",
		"Element/Kisame/Kisame.xml",
		"Element/Konan/Konan.xml",
		"Element/Kurama/Kurama.xml",
		"Element/Lee/Lee.xml",
		"Element/MaskFudon/MaskFudon.xml",
		"Element/MaskKadon/MaskKadon.xml",
		"Element/MaskRaidon/MaskRaidon.xml",
		"Element/Minato/Minato.xml",
		"Element/Naruto/Naruto.xml",
		"Element/Neji/Neji.xml",
		"Element/Parents/Parents.xml",
		"Element/RikudoNaruto/RikudoNaruto.xml",
		"Element/RockLee/RockLee.xml",
		"Element/Roshi/Roshi.xml",
		"Element/SageJiraiya/SageJiraiya.xml",
		"Element/SageNaruto/SageNaruto.xml",
		"Element/Sai/Sai.xml",
		"Element/Sakura/Sakura.xml",
		"Element/Sanshouuo/Sanshouuo.xml",
		"Element/Saso/Saso.xml",
		"Element/Sasuke/Sasuke.xml",
		"Element/Shikamaru/Shikamaru.xml",
		"Element/Shino/Shino.xml",
		"Element/Slug/Slug.xml",
		"Element/Suigetsu/Suigetsu.xml",
		"Element/Tenten/Tenten.xml",
		"Element/Tobi/Tobi.xml",
		"Element/Tobirama/Tobirama.xml",
		"Element/Tsunade/Tsunade.xml",

		"Tiles/1.tmx",
		"Tiles/2.tmx",
		"Tiles/3.tmx",
		"Tiles/4.tmx",
		"Tiles/5.tmx",
		"rank.json",
		"rank2.json",


	};

	std::string filePath = FileUtils::getInstance()->getWritablePath() + "checkMD5.xml";
	//xml�ĵ�
	tinyxml2::XMLDocument* pDoc = new tinyxml2::XMLDocument();  
	if (NULL==pDoc) {
		return ;
	}
	//xml����
	XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	if (NULL==pDel) {
		return ;
	}

	pDoc->LinkEndChild(pDel);
	//�ڵ�file
	XMLElement *fileElement = pDoc->NewElement("file");
	fileElement->SetAttribute("version", "1.18");//���ڵ���������
	pDoc->LinkEndChild(fileElement);




	//�ڵ�path
	int num= sizeof(fileList)/sizeof(char*);
	for(int i=0;i<num;i++){
		XMLElement *pathElement = pDoc->NewElement("path");
		pathElement->SetAttribute("src", fileList[i]);
		std::string fileSrc=fileList[i];
		std::string strFileMD5 =CMD5Checksum::GetMD5(fileSrc);
		pathElement->LinkEndChild(pDoc->NewText(strFileMD5.c_str()));
		fileElement->LinkEndChild(pathElement);
	}


	pDoc->SaveFile(filePath.c_str());//�����ļ� ������·��
	delete pDoc;


}

void  KTools::prepareFileSHA(){

	char* fileList[]={
		"Element/Bullet/Amaterasu.xml",
		"Element/Bullet/FlyKnife.xml",
		"Element/Bullet/HiraishinKunai.xml",
		"Element/Bullet/HugeSRK.xml",
		"Element/Bullet/Kusuri.xml",
		"Element/Bullet/PaperSpear.xml",
		"Element/Bullet/PaperSrk.xml",
		"Element/Bullet/Shintenshin.xml",
		"Element/Bullet/TentenSRK.xml",

		"Element/Monster/Bikyu.xml",
		"Element/Monster/Bug.xml",
		"Element/Monster/BugPillar.xml",
		"Element/Monster/BugTomado.xml",
		"Element/Monster/ChuiDi.xml",
		"Element/Monster/CircleMark.xml",
		"Element/Monster/ClayBird.xml",
		"Element/Monster/Crash.xml",
		"Element/Monster/Crash2.xml",
		"Element/Monster/DeidaraBom.xml",
		"Element/Monster/Dogs.xml",
		"Element/Monster/FakeDeidara.xml",
		"Element/Monster/FakeItachi.xml",
		"Element/Monster/FakeMinato.xml",
		"Element/Monster/FakeShino.xml",
		"Element/Monster/FakeTobirama.xml",
		"Element/Monster/FireRain.xml",
		"Element/Monster/FudonSRK.xml",
		"Element/Monster/FudonSRK2.xml",
		"Element/Monster/Hasan.xml",
		"Element/Monster/HiraishinMark.xml",
		"Element/Monster/InkBird.xml",
		"Element/Monster/InkDragon.xml",
		"Element/Monster/ItachiSusano.xml",
		"Element/Monster/Kage.xml",
		"Element/Monster/KageBom.xml",
		"Element/Monster/KageFeng.xml",
		"Element/Monster/KageHand.xml",
		"Element/Monster/KageHands.xml",
		"Element/Monster/Kaiten.xml",
		"Element/Monster/Kubi.xml",
		"Element/Monster/Kuroari.xml",
		"Element/Monster/LeeBom.xml",
		"Element/Monster/Mine.xml",
		"Element/Monster/Mouse.xml",
		"Element/Monster/Paku.xml",
		"Element/Monster/PaperRain.xml",
		"Element/Monster/Qilin.xml",
		"Element/Monster/QuanRen.xml",
		"Element/Monster/Sabaku.xml",
		"Element/Monster/SakuraBom.xml",
		"Element/Monster/Sanbao.xml",
		"Element/Monster/SandBall.xml",
		"Element/Monster/SandHand.xml",
		"Element/Monster/SandWave.xml",
		"Element/Monster/SasukeSusano.xml",
		"Element/Monster/Shark.xml",
		"Element/Monster/Shark2.xml",
		"Element/Monster/Shenwei.xml",
		"Element/Monster/Shenwei2.xml",
		"Element/Monster/Shoryu.xml",
		"Element/Monster/SmallSlug.xml",
		"Element/Monster/Snake.xml",
		"Element/Monster/Spider.xml",
		"Element/Monster/Steam.xml",
		"Element/Monster/Suiji.xml",
		"Element/Monster/Suijin.xml",
		"Element/Monster/SuiRyuDan.xml",
		"Element/Monster/TamaBomb.xml",
		"Element/Monster/Tenmu.xml",
		"Element/Monster/ThunderWave.xml",
		"Element/Monster/Tiger.xml",
		"Element/Monster/TodonPillar.xml",
		"Element/Monster/Traps.xml",
		"Element/Monster/Tsukuyomi.xml",
		"Element/Monster/Tuji.xml",
		"Element/Monster/Tuji2.xml",
		"Element/Monster/WaterBom.xml",
		"Element/Monster/WaterBullet.xml",
		"Element/Monster/Wave.xml",
		"Element/Monster/Yataikuzu.xml",
		"Element/Monster/Yominuma.xml",

		"Element/Flog/ObitoFlog.xml",
		"Element/Flog/FemalePainFlog.xml",
		"Element/Flog/IzumoFlog.xml",
		"Element/Flog/KakashiFlog.xml",
		"Element/Flog/KotetsuFlog.xml",
		"Element/Flog/PainFlog.xml",
		"Element/Tower/CenterDate.xml",
		"Element/Tower/TowerDate.xml",

		"Element/Akamaru/Akamaru.xml",
		"Element/Asuma/Asuma.xml",
		"Element/Centipede/Centipede.xml",
		"Element/Chiyo/Chiyo.xml",
		"Element/Choji/Choji.xml",
		"Element/Deidara/Deidara.xml",
		"Element/DogWall/DogWall.xml",
		"Element/Gaara/Gaara.xml",
		"Element/Han/Han.xml",
		"Element/Hidan/Hidan.xml",
		"Element/Hinata/Hinata.xml",
		"Element/Hiruzen/Hiruzen.xml",
		"Element/ImmortalSasuke/ImmortalSasuke.xml",
		"Element/Ino/Ino.xml",
		"Element/Itachi/Itachi.xml",
		"Element/Jiraiya/Jiraiya.xml",
		"Element/Jugo/Jugo.xml",
		"Element/Kakashi/Kakashi.xml",
		"Element/Kakuzu/Kakuzu.xml",
		"Element/Kankuro/Kankuro.xml",
		"Element/Karasu/Karasu.xml",
		"Element/Karin/Karin.xml",
		"Element/Kiba/Kiba.xml",
		"Element/Kisame/Kisame.xml",
		"Element/Konan/Konan.xml",
		"Element/Kurama/Kurama.xml",
		"Element/Lee/Lee.xml",
		"Element/MaskFudon/MaskFudon.xml",
		"Element/MaskKadon/MaskKadon.xml",
		"Element/MaskRaidon/MaskRaidon.xml",
		"Element/Minato/Minato.xml",
		"Element/Naruto/Naruto.xml",
		"Element/Neji/Neji.xml",
		"Element/Parents/Parents.xml",
		"Element/RikudoNaruto/RikudoNaruto.xml",
		"Element/RockLee/RockLee.xml",
		"Element/Roshi/Roshi.xml",
		"Element/SageJiraiya/SageJiraiya.xml",
		"Element/SageNaruto/SageNaruto.xml",
		"Element/Sai/Sai.xml",
		"Element/Sakura/Sakura.xml",
		"Element/Sanshouuo/Sanshouuo.xml",
		"Element/Saso/Saso.xml",
		"Element/Sasuke/Sasuke.xml",
		"Element/Shikamaru/Shikamaru.xml",
		"Element/Shino/Shino.xml",
		"Element/Slug/Slug.xml",
		"Element/Suigetsu/Suigetsu.xml",
		"Element/Tenten/Tenten.xml",
		"Element/Tobi/Tobi.xml",
		"Element/Tobirama/Tobirama.xml",
		"Element/Tsunade/Tsunade.xml"
	};

	std::string filePath = FileUtils::getInstance()->getWritablePath() + "SHA.xml";
	//xml�ĵ�
	tinyxml2::XMLDocument* pDoc = new tinyxml2::XMLDocument();  
	if (NULL==pDoc) {
		return ;
	}
	//xml����
	XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	if (NULL==pDel) {
		return ;
	}

	pDoc->LinkEndChild(pDel);
	//�ڵ�file
	XMLElement *fileElement = pDoc->NewElement("file");
	fileElement->SetAttribute("version", "1.18");//���ڵ���������
	pDoc->LinkEndChild(fileElement);




	//�ڵ�path
	int num= sizeof(fileList)/sizeof(char*);
	for(int i=0;i<num;i++){
		XMLElement *pathElement = pDoc->NewElement("path");
		pathElement->SetAttribute("src", fileList[i]);
		std::string fileSrc=fileList[i];
		std::string strFileMD5 =CMD5Checksum::GetMD5(fileSrc);
		pathElement->LinkEndChild(pDoc->NewText(strFileMD5.c_str()));
		

		fileElement->LinkEndChild(pathElement);
	}


	pDoc->SaveFile(filePath.c_str());//�����ļ� ������·��
	delete pDoc;


}


int KTools::checkMD5(std::string findPath /* ="" */){
	//MD5 
	
	std::string md5Path =  "Element/md5.xml";

	bool isExisted=FileUtils::getInstance()->isFileExist(md5Path);

	if(!isExisted){
		return 0;
	}
	//�Ա�MD5
	std::string officalFileMD5 =CMD5Checksum::GetMD5(md5Path);  
	


	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();  
	ssize_t nSize;
	const char *pXmlBuffer = (const char*)FileUtils::getInstance()->getFileData(md5Path.c_str(), "r", &nSize);
	doc->Parse(pXmlBuffer);

	XMLElement *rootEle=doc->RootElement();
	XMLElement *fileEle=rootEle->FirstChildElement();


	char* fileList[]={
		"Element/Bullet/Amaterasu.xml",
		"Element/Bullet/FlyKnife.xml",
		"Element/Bullet/HiraishinKunai.xml",
		"Element/Bullet/HugeSRK.xml",
		"Element/Bullet/Kusuri.xml",
		"Element/Bullet/PaperSpear.xml",
		"Element/Bullet/PaperSrk.xml",
		"Element/Bullet/Shintenshin.xml",
		"Element/Bullet/TentenSRK.xml",

		"Element/Monster/Bikyu.xml",
		"Element/Monster/Bug.xml",
		"Element/Monster/BugPillar.xml",
		"Element/Monster/BugTomado.xml",
		"Element/Monster/ChuiDi.xml",
		"Element/Monster/CircleMark.xml",
		"Element/Monster/ClayBird.xml",
		"Element/Monster/Crash.xml",
		"Element/Monster/Crash2.xml",
		"Element/Monster/DeidaraBom.xml",
		"Element/Monster/Dogs.xml",
		"Element/Monster/FakeDeidara.xml",
		"Element/Monster/FakeItachi.xml",
		"Element/Monster/FakeMinato.xml",
		"Element/Monster/FakeShino.xml",
		"Element/Monster/FakeTobirama.xml",
		"Element/Monster/FireRain.xml",
		"Element/Monster/FudonSRK.xml",
		"Element/Monster/FudonSRK2.xml",
		"Element/Monster/Hasan.xml",
		"Element/Monster/HiraishinMark.xml",
		"Element/Monster/InkBird.xml",
		"Element/Monster/InkDragon.xml",
		"Element/Monster/ItachiSusano.xml",
		"Element/Monster/Kage.xml",
		"Element/Monster/KageBom.xml",
		"Element/Monster/KageFeng.xml",
		"Element/Monster/KageHand.xml",
		"Element/Monster/KageHands.xml",
		"Element/Monster/Kaiten.xml",
		"Element/Monster/Kubi.xml",
		"Element/Monster/Kuroari.xml",
		"Element/Monster/LeeBom.xml",
		"Element/Monster/Mine.xml",
		"Element/Monster/Mouse.xml",
		"Element/Monster/Paku.xml",
		"Element/Monster/PaperRain.xml",
		"Element/Monster/Qilin.xml",
		"Element/Monster/QuanRen.xml",
		"Element/Monster/Sabaku.xml",
		"Element/Monster/SakuraBom.xml",
		"Element/Monster/Sanbao.xml",
		"Element/Monster/SandBall.xml",
		"Element/Monster/SandHand.xml",
		"Element/Monster/SandWave.xml",
		"Element/Monster/SasukeSusano.xml",
		"Element/Monster/Shark.xml",
		"Element/Monster/Shark2.xml",
		"Element/Monster/Shenwei.xml",
		"Element/Monster/Shenwei2.xml",
		"Element/Monster/Shoryu.xml",
		"Element/Monster/SmallSlug.xml",
		"Element/Monster/Snake.xml",
		"Element/Monster/Spider.xml",
		"Element/Monster/Steam.xml",
		"Element/Monster/Suiji.xml",
		"Element/Monster/Suijin.xml",
		"Element/Monster/SuiRyuDan.xml",
		"Element/Monster/TamaBomb.xml",
		"Element/Monster/Tenmu.xml",
		"Element/Monster/ThunderWave.xml",
		"Element/Monster/Tiger.xml",
		"Element/Monster/TodonPillar.xml",
		"Element/Monster/Traps.xml",
		"Element/Monster/Tsukuyomi.xml",
		"Element/Monster/Tuji.xml",
		"Element/Monster/Tuji2.xml",
		"Element/Monster/WaterBom.xml",
		"Element/Monster/WaterBullet.xml",
		"Element/Monster/Wave.xml",
		"Element/Monster/Yataikuzu.xml",
		"Element/Monster/Yominuma.xml",

		"Element/Flog/ObitoFlog.xml",
		"Element/Flog/FemalePainFlog.xml",
		"Element/Flog/IzumoFlog.xml",
		"Element/Flog/KakashiFlog.xml",
		"Element/Flog/KotetsuFlog.xml",
		"Element/Flog/PainFlog.xml",
		"Element/Tower/CenterDate.xml",
		"Element/Tower/TowerDate.xml",

		"Element/Akamaru/Akamaru.xml",
		"Element/Asuma/Asuma.xml",
		"Element/Centipede/Centipede.xml",
		"Element/Chiyo/Chiyo.xml",
		"Element/Choji/Choji.xml",
		"Element/Deidara/Deidara.xml",
		"Element/DogWall/DogWall.xml",
		"Element/Gaara/Gaara.xml",
		"Element/Han/Han.xml",
		"Element/Hidan/Hidan.xml",
		"Element/Hinata/Hinata.xml",
		"Element/Hiruzen/Hiruzen.xml",
		"Element/ImmortalSasuke/ImmortalSasuke.xml",
		"Element/Ino/Ino.xml",
		"Element/Itachi/Itachi.xml",
		"Element/Jiraiya/Jiraiya.xml",
		"Element/Jugo/Jugo.xml",
		"Element/Kakashi/Kakashi.xml",
		"Element/Kakuzu/Kakuzu.xml",
		"Element/Kankuro/Kankuro.xml",
		"Element/Karasu/Karasu.xml",
		"Element/Karin/Karin.xml",
		"Element/Kiba/Kiba.xml",
		"Element/Kisame/Kisame.xml",
		"Element/Konan/Konan.xml",
		"Element/Kurama/Kurama.xml",
		"Element/Lee/Lee.xml",
		"Element/MaskFudon/MaskFudon.xml",
		"Element/MaskKadon/MaskKadon.xml",
		"Element/MaskRaidon/MaskRaidon.xml",
		"Element/Minato/Minato.xml",
		"Element/Naruto/Naruto.xml",
		"Element/Neji/Neji.xml",
		"Element/Parents/Parents.xml",
		"Element/RikudoNaruto/RikudoNaruto.xml",
		"Element/RockLee/RockLee.xml",
		"Element/Roshi/Roshi.xml",
		"Element/SageJiraiya/SageJiraiya.xml",
		"Element/SageNaruto/SageNaruto.xml",
		"Element/Sai/Sai.xml",
		"Element/Sakura/Sakura.xml",
		"Element/Sanshouuo/Sanshouuo.xml",
		"Element/Saso/Saso.xml",
		"Element/Sasuke/Sasuke.xml",
		"Element/Shikamaru/Shikamaru.xml",
		"Element/Shino/Shino.xml",
		"Element/Slug/Slug.xml",
		"Element/Suigetsu/Suigetsu.xml",
		"Element/Tenten/Tenten.xml",
		"Element/Tobi/Tobi.xml",
		"Element/Tobirama/Tobirama.xml",
		"Element/Tsunade/Tsunade.xml",

		"Tiles/1.tmx",
		"Tiles/2.tmx",
		"Tiles/3.tmx",
		"Tiles/4.tmx",
		"Tiles/5.tmx",
		"rank.json",
		"rank2.json",

	};
	

	int i=0;

	
	while (fileEle){
		
		const char* pathSrc=fileEle->FirstAttribute()->Value();
		std::string pathMD5= fileEle->GetText();	
		std::string strFileMD5 =CMD5Checksum::GetMD5(fileList[i]);  
		std::string fileName=fileList[i];
		if(strcmp(pathSrc,fileList[i])==0){
			if(findPath!=""){
				if(strcmp(findPath.c_str(),fileName.c_str())==0){
					return i;
				}
				
			}
			if(strFileMD5==pathMD5){
			}else{
				return 0;
			}
		}else{
				return 0;
		}


		fileEle=fileEle->NextSiblingElement();
		i++;
	}

	return 1;

}





std::string  KTools::getKeycode(std::string path){
	std::string strKey =CMD5Checksum::GetMD5(path);
	std::string strKeyCode=cocos2d::StringUtils::format("%s,%s",strKey.substr(16,5).c_str(),strKey.substr(6,5).c_str());
	return strKeyCode ;
}

bool KTools::checkData(){

	const std::string filePath = FileUtils::getInstance()->getWritablePath() + "CheckMD5_113.xml";

	bool isExisted=FileUtils::getInstance()->isFileExist(filePath);


	if(!isExisted){
		return false;
	}

	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();  
	ssize_t nSize;
	const char *pXmlBuffer = (const char*)FileUtils::getInstance()->getFileData(filePath.c_str(), "r", &nSize);
	doc->Parse(pXmlBuffer);

	XMLElement *rootEle=doc->RootElement();
	XMLElement *fileEle=rootEle->FirstChildElement();

	int i=0;

	if(fileEle==NULL){
		CCLOG("%s node ERROR!");
		return false;
	}

	while (fileEle){

		std::string pathMD5= fileEle->GetText();	
		std::string savePath = FileUtils::getInstance()->getWritablePath() + "sql.db";
		std::string strFileMD5 =CMD5Checksum::GetMD5(savePath);


		if(strFileMD5==pathMD5){

		}else{
			return false;
		}

		fileEle=fileEle->NextSiblingElement();
		i++;

	}

	return true;


}




void KTools::updateData(){

	std::string filePath = FileUtils::getInstance()->getWritablePath() + "CheckMD5_113.xml";


	//xml�ĵ�
	tinyxml2::XMLDocument* pDoc = new tinyxml2::XMLDocument();  
	if (NULL==pDoc) {
		return ;
	}

	//xml����
	XMLDeclaration *pDel = pDoc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	if (NULL==pDel) {
		return ;
	}

	pDoc->LinkEndChild(pDel);
	//�ڵ�file
	XMLElement *fileElement = pDoc->NewElement("data");
	pDoc->LinkEndChild(fileElement);

	std::string savePath = FileUtils::getInstance()->getWritablePath() + "sql.db";
	std::string strFileMD5 =CMD5Checksum::GetMD5(savePath);
	XMLElement *pathElement = pDoc->NewElement("path");
	pathElement->LinkEndChild(pDoc->NewText(strFileMD5.c_str()));
	fileElement->LinkEndChild(pathElement);

	pDoc->SaveFile(filePath.c_str());
	delete pDoc;

}


bool KTools::readXMLToArray(const char* filePath,CCArray* &array)
{

	tinyxml2::XMLDocument* doc = new tinyxml2::XMLDocument();  
	//doc->LoadFile(filePath);  this's doesn't work for android
	ssize_t nSize;
	const char *data = (const char*)FileUtils::getInstance()->getFileData(filePath, "r", &nSize);
	doc->Parse(data);



	XMLElement *rootEle=doc->RootElement();
	XMLElement *actionEle=rootEle->FirstChildElement();

	
	while (actionEle){
		const char* actionName=actionEle->FirstAttribute()->Value();
		//CCLOG("[%s]",actionName);

		XMLElement *actionNodeEle=actionEle->FirstChildElement();
        CCArray* actionArray=CCArray::create();
		CCArray* actionDate=CCArray::create();
		CCArray* actionFrame=CCArray::create();
		while(actionNodeEle){
			//CCLOG("+[%s]",actionNodeEle->Name());
			if(strcmp(actionNodeEle->Name(),"frame")){
			    XMLElement *dateEle=actionNodeEle->FirstChildElement();

				while(dateEle){

					const char* nodeKey=dateEle->FirstAttribute()->Value();
					CCString* nodeValue=CCString::create(dateEle->GetText());

					//CCLOG("%s:%s",nodeKey,nodeValue->getCString());
					CCDictionary* dateDic=CCDictionary::create();
					dateDic->setObject(nodeValue,nodeKey);

					actionDate->addObject(dateDic);
					dateEle=dateEle->NextSiblingElement();
				}

			}else{
				
				XMLElement *frameEle=actionNodeEle->FirstChildElement();

				while(frameEle){
					const char* nodeKey;
					if(strcmp(frameEle->Name(),"frameName")){
						nodeKey=frameEle->FirstAttribute()->Value();

					}else{
						nodeKey=frameEle->Name();
					}
					
					CCString* nodeValue=CCString::create(frameEle->GetText());

					//CCLOG("%s:%s",nodeKey,nodeValue->getCString());
					CCDictionary* frameDic=CCDictionary::create();
					frameDic->setObject(nodeValue,nodeKey);
					actionFrame->addObject(frameDic);

					frameEle=frameEle->NextSiblingElement();
				}
				
			}
			actionNodeEle= actionNodeEle->NextSiblingElement();
		}
		actionArray->addObject(actionDate);
		actionArray->addObject(actionFrame);

		array->addObject(actionArray);
		actionEle=actionEle->NextSiblingElement();
		
	}

	delete data;
	
	return true;

};



int isExisted(void *para,int n_column,char **column_value,char **column_name){
	bool *isExisted_=(bool*)para; 
	*isExisted_=(**column_value)!='0'; 
	return 0;
}


void  KTools::encode(std::string &str,int radomKey){
	
	for(int i=0;i<str.length();i++) {
		int ch = str[i];
		ch+=radomKey;
		str[i]=ch;
	}
	str+=radomKey;
	
}


void  KTools::decode(std::string &str){
	int num=str.length()-1;
	int key=str[num];
	str=str.substr(0,num);

	for(int i=0;i<str.length();i++) {
		int ch = str[i];
		ch-=key;
		str[i]=ch;
	}


}


void KTools::initTableInDB(){

	sqlite3* pDB = NULL;
	char* errorMsg=NULL;
	std::string path=FileUtils::getInstance()->getWritablePath() +"sql.db";
	int result=sqlite3_open(path.c_str(),&pDB);  
	if(result!=SQLITE_OK){
		CCLOG("open sql file Failed!");
		return;
	};

	CCString*sql=CCString::create("drop table IF EXISTS Achievement");

	sqlite3_exec(pDB,sql->getCString(),NULL,NULL,&errorMsg);


	sql=CCString::create("create table if not exists CharRecord (name char(20)  primary key ,column1 char(10),column2 char(10),column3 char(10))");
	sqlite3_exec(pDB,sql->getCString(),NULL,NULL,&errorMsg);
	 
	sql=CCString::create("select * from  GameRecord");
	
	sqlite3_exec(pDB,sql->getCString(),NULL,NULL,&errorMsg);
	


	if(errorMsg!=NULL){
		errorMsg=NULL;
		sql=CCString::create("create table if not exists GameRecord (id char(10) primary key,coin char(20),version char(20))");
		sqlite3_exec(pDB,sql->getCString(),NULL,NULL,&errorMsg);
		
		std::string str="n>";
		CCString* coin=CCString::create(str);

		std::string insertSql=cocos2d::StringUtils::format("insert into GameRecord values(1,'%s','1')",coin->getCString());
		int result=sqlite3_exec(pDB,insertSql.c_str(),NULL,NULL,&errorMsg);

		if(errorMsg!=NULL){
			
			CCLOG("exec sql %s failed with mgs: %s",insertSql.c_str(),errorMsg);
			sqlite3_close(pDB);
			return;
		}

	
		this->updateData();
	}

	
	bool isExisted_;
	
	sql=CCString::create("select count(*) from  CharRecord");
	sqlite3_exec(pDB,sql->getCString(),isExisted,&isExisted_,&errorMsg);
	
	if(!isExisted_){

		char* heroList[]={"Konan","Sakura","Naruto","Sai","Deidara",
			"Kakashi","Itachi","Tenten","Jiraiya","Suigetsu",
			"Tsunade","Tobirama","Neji","Ino","Asuma","Gaara",
			"Karin","Sasuke","Hidan","Choji","Kankuro",
			"Shino","Minato","Tobi","Kakuzu","Hinata",
			"Shikamaru","Chiyo","Kisame",
			"Hiruzen","Kiba","Jugo","Lee"
		};
		int num= sizeof(heroList)/sizeof(char*);

		for(int i=0;i<num;i++){

			std::string name=heroList[i];

			int key=rand()%50+40;
			encode(name,key);
			CCString* nameDB=CCString::create(name);
		

			std::string column1="0";
			key=rand()%60+40;
			encode(column1,key);
			CCString* column1DB=CCString::create(column1);

			std::string column2="0";
			key=rand()%60+40;
			encode(column2,key);
			CCString* column2DB=CCString::create(column2);


			std::string column3="";
			key=rand()%60+40;
			encode(column3,key);
			CCString* column3DB=CCString::create(column3);




			std::string insertSql=cocos2d::StringUtils::format("insert into  CharRecord values('%s','%s','%s','%s')",nameDB->getCString(),column1DB->getCString(),column2DB->getCString(),column3DB->getCString());
			sqlite3_exec(pDB,insertSql.c_str(),NULL,NULL,&errorMsg);

			if(errorMsg!=NULL){
				CCLOG("exec sql %s failed with mgs: %s",insertSql.c_str(),errorMsg);
				sqlite3_close(pDB);
				return;
			}

		}

		this->updateData();

	}

	sqlite3_close(pDB);
}


void KTools::initColumeInDB(){

	sqlite3* pDB = NULL;
	char* errorMsg=NULL;
	std::string path=FileUtils::getInstance()->getWritablePath() +"sql.db";
	int result=sqlite3_open(path.c_str(),&pDB);  
	if(result!=SQLITE_OK){
		CCLOG("open sql file Failed!");
		return;
	};

	
	char **result2;
	int row=0;
	int column=0;
	
	CCString* sql=CCString::create("select coin from GameRecord");
	sqlite3_get_table(pDB,sql->getCString(),&result2,&row,&column,NULL);
	std::string str2=result2[1];
	decode(str2);

	
	std::string str3="uuuuu<";
	decode(str3);
	if(std::atoi(str2.c_str())>std::atoi(str3.c_str())){
		CCString* coin=CCString::create("uuuuu<");
		std::string updateSql=cocos2d::StringUtils::format("update GameRecord set coin='%s'",coin->getCString());
		sqlite3_exec(pDB,updateSql.c_str(),NULL,NULL,&errorMsg);
		this->updateData();
	}

	/*std::string folderPath =FileUtils::getInstance()->getWritablePath()+"DLC";
	this->dfsFolder(folderPath);*/
	
	

	
}



void KTools::dfsFolder(std::string folderPath, int depth /* = 0 */,int type/* =0 */){

#ifdef WIN32
	_finddata_t FileInfo;
	std::string strfind = folderPath + "\\*";
	long Handle = _findfirst(strfind.c_str(), &FileInfo);

	if (Handle == -1L)
	{
		
		exit(-1);
	}
	do{
		//�ж��Ƿ�����Ŀ¼
		if (FileInfo.attrib & _A_SUBDIR)    
		{
			//���������Ҫ
			if( (strcmp(FileInfo.name,".") != 0 ) &&(strcmp(FileInfo.name,"..") != 0))   
			{
				std::string newPath = folderPath + "\\" + FileInfo.name;
				if(type==0){
				this->dfsFolder(newPath,0);
				}else if(type==1){
					this->dfsFolder(newPath,0,1);
				}
			}
		}
		else  
		{			
			if(type==0){
				std::string filename = (folderPath + "\\" + FileInfo.name);
				std:remove(filename.c_str());
			}else if(type==1){
				std::string filename = (folderPath +"\\" + FileInfo.name);
				CCLOG("<path src=\"%s\">%s</path>",folderPath.substr(6).c_str(),filename.c_str());
				
			}
			
			//std::cout << folderPath << "\\" << FileInfo.name  << " " << endl;
		}
	}while (_findnext(Handle, &FileInfo) == 0);

	_findclose(Handle);
#else
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	if((dp = opendir(folderPath.c_str())) == NULL) {
		//CCLOG("damn:%s",folderPath.c_str());
		//fprintf(stderr,"cannot open directory: %s\n", folderPath.c_str());
		return;
	}
	chdir(folderPath.c_str());
	while((entry = readdir(dp)) != NULL) {
		
		lstat(entry->d_name,&statbuf);
		if(S_ISDIR(statbuf.st_mode)) {
			
			if(strcmp(".",entry->d_name) == 0 ||
				strcmp("..",entry->d_name) == 0)
				continue;
			if(type==0){
			dfsFolder(entry->d_name,depth+4);
			}else {
				dfsFolder(entry->d_name,depth+4,1);
			}
		} else {
			std::string filename = entry->d_name;
			if(type==0){
			remove(filename.c_str());
			}else if(type==1){
				
				//CCLOG("damn3:%s",filename.c_str());
				SimpleAudioEngine::getInstance()->preloadEffect(filename.c_str());
			}
			
		}
	}
	chdir("..");
	closedir(dp);
#endif

}

static bool sAudioListLoaded = false;
static std::map<std::string, std::vector<std::string> > sAudioListCache;
static std::map<std::string, bool> sPreloadedEffects;
static std::map<std::string, unsigned int> sLastEffectFrame;
static unsigned int sEffectFrame = 0;
static int sEffectsPlayedThisFrame = 0;

static void loadAudioListCache()
{
    if (sAudioListLoaded)
        return;

    sAudioListLoaded = true;

    std::string md5Path = "Audio/list.xml";

    if (!FileUtils::getInstance()->isFileExist(md5Path))
        return;

    ssize_t nSize = 0;
    unsigned char* pXmlBuffer = FileUtils::getInstance()->getFileData(md5Path.c_str(), "r", &nSize);

    if (!pXmlBuffer || nSize <= 0)
        return;

    tinyxml2::XMLDocument doc;
    doc.Parse(reinterpret_cast<const char*>(pXmlBuffer), nSize);

    CC_SAFE_DELETE_ARRAY(pXmlBuffer);

    XMLElement* rootEle = doc.RootElement();
    if (!rootEle)
        return;

    XMLElement* fileEle = rootEle->FirstChildElement();

    while (fileEle)
    {
        if (fileEle->FirstAttribute() && fileEle->GetText())
        {
            const char* listName = fileEle->FirstAttribute()->Value();
            const char* soundPath = fileEle->GetText();

            if (listName && soundPath)
            {
                std::string normalizedPath = soundPath;
                std::replace(normalizedPath.begin(), normalizedPath.end(), '\\', '/');
                sAudioListCache[listName].push_back(normalizedPath);
            }
        }

        fileEle = fileEle->NextSiblingElement();
    }
}

void KTools::prepareFileOGG(const char* listName, int type /* = 0 */)
{
    if (!UserDefault::getInstance()->getBoolForKey("isPreload"))
        return;

    if (!listName || !listName[0])
        return;

    loadAudioListCache();

    std::map<std::string, std::vector<std::string> >::iterator it = sAudioListCache.find(listName);

    if (it == sAudioListCache.end())
        return;


    const std::vector<std::string>& files = it->second;

    for (size_t i = 0; i < files.size(); ++i)
    {
        const char* soundPath = files[i].c_str();

        if (type == 0)
        {
            KTools::preloadEffectCached(soundPath);
        }
        else if (type == 1)
        {
            KTools::unloadEffectCached(soundPath);
        }
    }
}

void KTools::preloadEffectCached(const char* soundPath)
{
    if (!soundPath || !soundPath[0])
        return;

    std::string key = soundPath;
    std::replace(key.begin(), key.end(), '\\', '/');
    if (sPreloadedEffects[key])
        return;

    SimpleAudioEngine::getInstance()->preloadEffect(key.c_str());
    sPreloadedEffects[key] = true;
}

void KTools::unloadEffectCached(const char* soundPath)
{
    if (!soundPath || !soundPath[0])
        return;

    std::string key = soundPath;
    std::replace(key.begin(), key.end(), '\\', '/');
    SimpleAudioEngine::getInstance()->unloadEffect(key.c_str());
    sPreloadedEffects.erase(key);
    sLastEffectFrame.erase(key);
}

unsigned int KTools::playEffectOptimized(const char* soundPath, unsigned int minFrameGap, int maxPerFrame)
{
    if (!soundPath || !soundPath[0])
        return 0;

    unsigned int frame = Director::getInstance()->getTotalFrames();
    if (frame != sEffectFrame)
    {
        sEffectFrame = frame;
        sEffectsPlayedThisFrame = 0;
    }

    if (maxPerFrame > 0 && sEffectsPlayedThisFrame >= maxPerFrame)
        return 0;

    std::string key = soundPath;
    std::replace(key.begin(), key.end(), '\\', '/');
    std::map<std::string, unsigned int>::iterator it = sLastEffectFrame.find(key);
    if (it != sLastEffectFrame.end() && frame - it->second < minFrameGap)
        return 0;

    if (UserDefault::getInstance()->getBoolForKey("isPreload"))
        KTools::preloadEffectCached(soundPath);

    sLastEffectFrame[key] = frame;
    ++sEffectsPlayedThisFrame;
    return SimpleAudioEngine::getInstance()->playEffect(key.c_str());
}


bool KTools::checkPackage(){

	std::string* packageList[10];

//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//	JniMethodInfo minfo;
//	bool isHave = JniHelper::getMethodInfo(minfo,"net/zakume/game/NarutoSenki","getPackages", "([Ljava/lang/String;I)[Ljava/lang/String");
//
//    if (isHave) {
//
//    }
//
//
//#endif
	return true;

}









sqlite3* KTools::prepareTableInDB(){

	sqlite3* pDB = NULL;
	char* errorMsg=NULL;

	std::string path=FileUtils::getInstance()->getWritablePath() +"sql.db";
	int result=sqlite3_open(path.c_str(),&pDB);  

	if(result!=SQLITE_OK){
		CCLOG("open sql file Failed!");
		return NULL;
	};

	return pDB;


}
bool KTools::saveToSQLite(const char* table/* ="GameRecord"*/,const char* column/* =NULL */,const char* value/* =NULL */,bool isBuy /*= false */){

	sqlite3* pDB =prepareTableInDB();
	char* errorMsg=NULL;
	if(pDB!=NULL){

		int key=rand()%60+40;
		std::string str=value;


		encode(str,key);
		CCString* coin=CCString::create(str);
		std::string sql=cocos2d::StringUtils::format("update %s set %s='%s'",table,column,coin->getCString());
		sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errorMsg);

		if(errorMsg!=NULL){
			sqlite3_close(pDB);
			return false;
		}
		
		this->updateData();	

		return true;
	}

	return false;
	sqlite3_close(pDB);
}




CCString* KTools::readFromSQLite(const char* table/* ="GameRecord" */,const char* column/* =NULL */,const char* value/* =NULL */){

	sqlite3* pDB =prepareTableInDB();


	if(pDB!=NULL){

		char **result;
		std::string sql="select coin from  GameRecord";
		int row=0;
		int column=0;

		sqlite3_get_table(pDB,sql.c_str(),&result,&row,&column,NULL);

		std::string str=result[1];
		decode(str);
		CCString* coin=CCString::create(str);
		sqlite3_free_table(result);
		sqlite3_close(pDB);
		return coin;
	}
	sqlite3_close(pDB);
	return 0;

}

CCString* KTools::readSQLite(const char* table,const char* column,const char* value,const char* targetColumn){

	sqlite3* pDB =prepareTableInDB();


	if(pDB!=NULL){

		char **result;

		std::string sql=cocos2d::StringUtils::format("select %s,%s from  %s ",column,targetColumn,table);
		int row=0;
		int column=0;

		sqlite3_get_table(pDB,sql.c_str(),&result,&row,&column,NULL);
		 
		std::string target;
		for( int i = 0; i <= row*2 ; i++ ){
			
			std::string str=result[i];

			decode(str);
			if(strcmp(str.c_str(),value)==0){
				target=result[i+1];
				decode(target);
				break;
			}
	
		}

		CCString* targetValue=CCString::create("");


		if(strcmp(targetColumn,"column3")==0 || strcmp(targetColumn,"column4")==0){
		    targetValue=CCString::create(target);
		}else{
			std::string targetText=cocos2d::StringUtils::format("%d",std::atoi(target.c_str()));
			targetValue=CCString::create(targetText);
		}
		sqlite3_free_table(result);
		sqlite3_close(pDB);
		return targetValue;
		
	}
	sqlite3_close(pDB);
	return 0;
	
}

void KTools::saveSQLite(const char* table,const char* relatedColumn,const char* value,const char* targetColumn,char* targetValue,bool isPlus){
	
	char* errorMsg=NULL;
	sqlite3* pDB =prepareTableInDB();


	if(pDB!=NULL){

		char **result;

		std::string sql=cocos2d::StringUtils::format("select %s,%s from  %s ",relatedColumn,targetColumn,table);
		int row=0;
		int column=0;


		sqlite3_get_table(pDB,sql.c_str(),&result,&row,&column,NULL);

		std::string target;
		std::string columnValue;
		for( int i = 0; i <= row*2 ; i++ ){
			std::string str=result[i];
			decode(str);
			if(strcmp(str.c_str(),value)==0){
				columnValue=result[i];
				target=result[i+1];
				decode(target);
				break;
			}
		}

		
		std::string saveValue;
		if(isPlus){
			saveValue=std::string(targetValue)+ target;	
		
		}else{
			saveValue=std::string(targetValue);		
		}
		int key=rand()%50+40;
		encode(saveValue,key);
		sql=cocos2d::StringUtils::format("update %s set %s='%s' where %s='%s'",table,targetColumn,saveValue.c_str(),relatedColumn,columnValue.c_str());
		sqlite3_exec(pDB,sql.c_str(),NULL,NULL,NULL);
		
		
		if(isPlus){
			char **result2;
			sql="select coin from GameRecord";
			sqlite3_get_table(pDB,sql.c_str(),&result2,&row,&column,NULL);
			std::string str2=result2[1];
			decode(str2);

			std::string str3="uuuuu<";
			decode(str3);

			if(std::atoi(str2.c_str())>std::atoi(str3.c_str())){
				CCString* coin=CCString::create("uuuuu<");
				sql=cocos2d::StringUtils::format("update GameRecord set coin='%s'",coin->getCString());
				sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errorMsg);
			}
		}
	
		sqlite3_free_table(result);


		this->updateData();

		sqlite3_close(pDB);
		return;
	}
	sqlite3_close(pDB);


}




std::string KTools::encodeData(std::string data){

	//SHA1 *sha1;  
	//unsigned char *digest;  
	std::string dataMD5 =CMD5Checksum::GetMD5OfString(data);

	//sha1=new SHA1;  
	//sha1->addBytes(dataMD5.c_str(),strlen(dataMD5.c_str()));//��������  
	////sha1->addBytes( "goldlion" ,strlen( "goldlion" ));//��������  
	//digest=sha1->getDigest();//��ȡУ����  
	//std::string result=(char*) digest;
	//delete sha1;  
	//free(digest);  
	return dataMD5;
}




CCTips::CCTips(void)
{

}


CCTips::~CCTips(void)
{
}

bool CCTips::init(const char *tips){
	bool bRet = false;
	do{
		CC_BREAK_IF(!Sprite::init());
		this->setAnchorPoint(Vec2(0.5f,0.5f));
		CCDictionary *strings = CCDictionary::createWithContentsOfFile("Element/strings.xml");  
		const char *reply = ((CCString*)strings->objectForKey(tips))->getCString();  

		Label* tipLabel=Label::createWithSystemFont(reply,FONT_TYPE,12);
		this->addChild(tipLabel,5000);
		this->setPosition(Vec2(winSize.width/2,50));
		FiniteTimeAction *call=CallFunc::create(CC_CALLBACK_0(CCTips::onDestroy, this));

		ActionInterval*  mv=MoveBy::create(0.2f,Vec2(0,12));
		ActionInterval* fadeOut=FadeOut::create(0.2f);
		DelayTime* delay=DelayTime::create(2.0f);
		ActionInterval* sp=Spawn::create(fadeOut,mv,NULL);
		Action* seq=Sequence::create(sp, delay, call, NULL);

		this->runAction(seq);


		bRet=true;

	}while(0);

	return bRet;

}

void CCTips::onDestroy(){
	this->removeAllChildren();
	this->removeFromParentAndCleanup(true);
}



CCTips*  CCTips::create( const char *tips ){
	CCTips* ab=new  CCTips();
	if (ab && ab->init(tips))
	{
		ab->autorelease();
		return ab;
	}
	else
	{
		delete ab;
		return NULL;
	}
}



