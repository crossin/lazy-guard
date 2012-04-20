#include "Level.h"
#include "libxml/tree.h"

using namespace cocos2d;

Level::Level(void)
{
}

Level::~Level(void)
{
	if (treasure)
	{
		treasure->release();
		treasure = NULL;
	}
	if (obstacles)
	{
		obstacles->release();
		obstacles = NULL;
	}
	if (guards)
	{
		guards->release();
		guards = NULL;
	}
	if (thieves)
	{
		thieves->release();
		thieves = NULL;
	}
}

Level* Level::level()
{
	Level *level = new Level;

	if (level && level->init())
	{
		level->autorelease();
		return level;
	}

	return NULL;
}

bool Level::init()
{
	bool bRet = false;
	do{
		obstacles = CCArray::array();
		obstacles->retain();
		guards = CCArray::array();
		guards->retain();
		thieves = CCArray::array();
		thieves->retain();
		treasure = new CCMutableDictionary<std::string, CCString*>();
		width = 15;
		height = 10;
		tileWidth = 32;
		tileHeight = 32;

		bRet=true;
	}while(0);

	return bRet;
}

bool Level::save()
{
	//�����ĵ��ͽڵ�ָ��
	xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");
	xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"level");

	//���ø��ڵ�
	xmlDocSetRootElement(doc,root_node);

	//�ڸ��ڵ���ֱ�Ӵ����ڵ�
// 	xmlNewTextChild(root_node, NULL, BAD_CAST "newNode1", BAD_CAST "newNode1 content");
// 	xmlNewTextChild(root_node, NULL, BAD_CAST "newNode2", BAD_CAST "newNode2 content");
// 	xmlNewTextChild(root_node, NULL, BAD_CAST "newNode3", BAD_CAST "newNode3 content");

	//����һ���ڵ㣬���������ݺ����ԣ�Ȼ���������
	xmlNodePtr node;
	xmlNodePtr son_node;
	xmlNodePtr content;
	char number[4];
	CCMutableDictionary<std::string, CCString*>* dic;
	vector<std::string> vc;
	vector<std::string>::iterator it;
	string key;

	// background
	node = xmlNewNode(NULL,BAD_CAST"background");
	sprintf_s(number, "%d", background);
	content = xmlNewText(BAD_CAST(number));
	xmlAddChild(root_node,node);
	xmlAddChild(node,content);

	// obstacle
	for (int i=0; i<obstacles->count(); i++)
	{
		node = xmlNewNode(NULL, BAD_CAST"obstacle");
		xmlAddChild(root_node, node);

		dic = (CCMutableDictionary<std::string, CCString*>*)obstacles->objectAtIndex(i);

		vc = dic->allKeys();
		for (it = vc.begin(); it != vc.end(); it++)
		{
			key = *it;
 			son_node = xmlNewNode(NULL, BAD_CAST(key.c_str()));
 			content = xmlNewText(BAD_CAST(dic->objectForKey(key)->toStdString().c_str()));
 			xmlAddChild(node,son_node);
 			xmlAddChild(son_node,content);
		}
	}
	
	// treasure
	node = xmlNewNode(NULL, BAD_CAST"treasure");
	xmlAddChild(root_node, node);
	vc = treasure->allKeys();
	for (it = vc.begin(); it != vc.end(); it++)
	{
		key = *it;
		son_node = xmlNewNode(NULL, BAD_CAST(key.c_str()));
		content = xmlNewText(BAD_CAST(treasure->objectForKey(key)->toStdString().c_str()));
		xmlAddChild(node,son_node);
		xmlAddChild(son_node,content);
	}

	// guard
	for (int i=0; i<guards->count(); i++)
	{
		node = xmlNewNode(NULL, BAD_CAST"guard");
		xmlAddChild(root_node, node);

		dic = (CCMutableDictionary<std::string, CCString*>*)guards->objectAtIndex(i);

		vc = dic->allKeys();
		for (it = vc.begin(); it != vc.end(); it++)
		{
			key = *it;
			son_node = xmlNewNode(NULL, BAD_CAST(key.c_str()));
			content = xmlNewText(BAD_CAST(dic->objectForKey(key)->toStdString().c_str()));
			xmlAddChild(node,son_node);
			xmlAddChild(son_node,content);
		}
	}

	// thief
	for (int i=0; i<thieves->count(); i++)
	{
		node = xmlNewNode(NULL, BAD_CAST"thief");
		xmlAddChild(root_node, node);

		dic = (CCMutableDictionary<std::string, CCString*>*)thieves->objectAtIndex(i);

		vc = dic->allKeys();
		for (it = vc.begin(); it != vc.end(); it++)
		{
			key = *it;
			son_node = xmlNewNode(NULL, BAD_CAST(key.c_str()));
			content = xmlNewText(BAD_CAST(dic->objectForKey(key)->toStdString().c_str()));
			xmlAddChild(node,son_node);
			xmlAddChild(son_node,content);
		}
	}

	//xmlNewProp(node,BAD_CAST"attribute",BAD_CAST "yes");

	/*
	//����һ�����Ӻ����ӽڵ�
	node = xmlNewNode(NULL, BAD_CAST "son");
	xmlAddChild(root_node,node);

	xmlNodePtr grandson = xmlNewNode(NULL, BAD_CAST "grandson");
	xmlAddChild(node,grandson);
	xmlAddChild(grandson, xmlNewText(BAD_CAST "This is a grandson node"));
*/
	//�洢xml�ĵ�
	int nRel = xmlSaveFormatFileEnc("level.xml",doc,"UTF-8",1);
	if (nRel == -1)
	{
		return false;
	}

	//�ͷ��ĵ��ڽڵ㶯̬������ڴ�
	xmlFreeDoc(doc);

	return true;
}

bool Level::load()
{
	xmlDocPtr doc;           //��������ĵ�ָ��
	xmlNodePtr curNode;      //������ָ��(����Ҫ��Ϊ���ڸ��������ƶ�)
	xmlNodePtr sonNode;
	//xmlNodePtr propsNode;
	//xmlChar *szKey;          //��ʱ�ַ�������
	char *szDocName;
	CCMutableDictionary<std::string, CCString*>* dic; 
	//CCMutableDictionary<std::string, CCString*>* props = new CCMutableDictionary<std::string, CCString*>();
	szDocName = "level.xml";
	doc = xmlReadFile(szDocName,"UTF-8",XML_PARSE_NOBLANKS); //�����ļ�
	//�������ĵ��Ƿ�ɹ���������ɹ���libxml��ָһ��ע��Ĵ���ֹͣ��
	//һ�����������ǲ��ʵ��ı��롣XML��׼�ĵ�������UTF-8��UTF-16�⻹�����������뱣�档
	//����ĵ���������libxml���Զ���Ϊ��ת����UTF-8���������XML������Ϣ������XML��׼��.
	if (NULL == doc)
	{
		return false;
	}

	curNode = xmlDocGetRootElement(doc); //ȷ���ĵ���Ԫ��
	/*���ȷ�ϵ�ǰ�ĵ��а�������*/
	if (NULL == curNode)
	{
		xmlFreeDoc(doc);
		return false;
	}

	/*����������У�������Ҫȷ���ĵ�����ȷ�����͡���root���������ʾ����ʹ���ĵ��ĸ����͡�*/
	if (xmlStrcmp(curNode->name, BAD_CAST "level"))
	{
		xmlFreeDoc(doc);
		return false;
	}

	curNode = curNode->xmlChildrenNode;
	//xmlNodePtr propNodePtr = curNode;
	
	while(curNode != NULL)
	{
		
		// get map
		/*if (!xmlStrcmp(curNode->name, (const xmlChar *)"map"))
		{
			obsNode = curNode->xmlChildrenNode;
			while(obsNode != NULL)
			{
				CCMutableDictionary<std::string, CCString*>* obsTemp;
				obsTemp = new CCMutableDictionary<std::string, CCString*>();
				propsNode = obsNode->xmlChildrenNode;
				while(propsNode != NULL)
				{
					obsTemp->setObject(new CCString((const char*)xmlNodeGetContent(propsNode)), string((char*)propsNode->name));
					propsNode = propsNode->next;
				}
				obstacles->addObject(obsTemp);
				obsNode = obsNode->next;
			}
		}*/

		if (!xmlStrcmp(curNode->name, (const xmlChar *)"obstacle"))
		{
			sonNode = curNode->xmlChildrenNode;
			//CCMutableDictionary<std::string, CCString*>* obsTemp = new CCMutableDictionary<std::string, CCString*>();
			dic = new CCMutableDictionary<std::string, CCString*>();
			while(sonNode != NULL)
			{
				CCString* str = new CCString((const char*)xmlNodeGetContent(sonNode));
				dic->setObject(str, string((char*)sonNode->name));
				str->autorelease();
				sonNode = sonNode->next;
			}
			obstacles->addObject(dic);
			dic->autorelease();
		}
		else if (!xmlStrcmp(curNode->name, (const xmlChar *)"treasure"))
		{
			sonNode = curNode->xmlChildrenNode;
			while(sonNode != NULL)
			{
				CCString* str = new CCString((const char*)xmlNodeGetContent(sonNode));
				treasure->setObject(str, string((char*)sonNode->name));
				str->autorelease();
				sonNode = sonNode->next;
			}
		}
		else if (!xmlStrcmp(curNode->name, (const xmlChar *)"guard"))
		{
			sonNode = curNode->xmlChildrenNode;
			dic = new CCMutableDictionary<std::string, CCString*>();
			while(sonNode != NULL)
			{
				CCString* str = new CCString((const char*)xmlNodeGetContent(sonNode));
				dic->setObject(str, string((char*)sonNode->name));
				str->autorelease();
				sonNode = sonNode->next;
			}
			guards->addObject(dic);
			dic->autorelease();
		}
		else if (!xmlStrcmp(curNode->name, (const xmlChar *)"thief"))
		{
			sonNode = curNode->xmlChildrenNode;
			dic = new CCMutableDictionary<std::string, CCString*>();
			while(sonNode != NULL)
			{
				CCString* str = new CCString((const char*)xmlNodeGetContent(sonNode));
				dic->setObject(str, string((char*)sonNode->name));
				str->autorelease();
				sonNode = sonNode->next;
			}
			thieves->addObject(dic);
			dic->autorelease();
		}
		else if (!xmlStrcmp(curNode->name, (const xmlChar *)"background"))
		{
			CCString* str = new CCString((const char*)xmlNodeGetContent(curNode));
			background = str->toInt();
			str->autorelease();
		}
		/*
		//ȡ���ڵ��е�����
		//if ((!xmlStrcmp(curNode->name, (const xmlChar *)"newNode1")))
		{
			szKey = xmlNodeGetContent(curNode);
			CCString* ss = new CCString((char*)szKey);
			xmlFree(szKey);
		}

		//���Ҵ�������attribute�Ľڵ�
		if (xmlHasProp(curNode,BAD_CAST "attribute"))
		{
			propNodePtr = curNode;
		}*/
		
		curNode = curNode->next;
	}

	/*
	//��������
	xmlAttrPtr attrPtr = propNodePtr->properties;
	while (attrPtr != NULL)
	{
		if (!xmlStrcmp(attrPtr->name, BAD_CAST "attribute"))
		{
			xmlChar* szAttr = xmlGetProp(propNodePtr,BAD_CAST "attribute");
			xmlFree(szAttr);
		}
		attrPtr = attrPtr->next;
	}*/

	xmlFreeDoc(doc);

	// get properties
	//background = props->objectForKey("background")->toInt();
	//props->autorelease();

	return true;
}
