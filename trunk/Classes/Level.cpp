#include "Level.h"
#include "libxml/tree.h"

using namespace cocos2d;

Level::Level(void)
{
}

Level::~Level(void)
{
	int a=1;
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
		bRet=true;
	}while(0);

	return bRet;
}

bool Level::save()
{
	//�����ĵ��ͽڵ�ָ��
	xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");
	xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"root");

	//���ø��ڵ�
	xmlDocSetRootElement(doc,root_node);

	//�ڸ��ڵ���ֱ�Ӵ����ڵ�
// 	xmlNewTextChild(root_node, NULL, BAD_CAST "newNode1", BAD_CAST "newNode1 content");
// 	xmlNewTextChild(root_node, NULL, BAD_CAST "newNode2", BAD_CAST "newNode2 content");
// 	xmlNewTextChild(root_node, NULL, BAD_CAST "newNode3", BAD_CAST "newNode3 content");

	//����һ���ڵ㣬���������ݺ����ԣ�Ȼ���������
	xmlNodePtr node = xmlNewNode(NULL,BAD_CAST"node2");
	xmlNodePtr content = xmlNewText(BAD_CAST"NODE CONTENT");
	xmlAddChild(root_node,node);
	xmlAddChild(node,content);
	xmlNewProp(node,BAD_CAST"attribute",BAD_CAST "yes");

	//����һ�����Ӻ����ӽڵ�
	node = xmlNewNode(NULL, BAD_CAST "son");
	xmlAddChild(root_node,node);

	xmlNodePtr grandson = xmlNewNode(NULL, BAD_CAST "grandson");
	xmlAddChild(node,grandson);
	xmlAddChild(grandson, xmlNewText(BAD_CAST "This is a grandson node"));

	//�洢xml�ĵ�
	int nRel = xmlSaveFileEnc("CreatedXml.xml",doc,"UTF-8");
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
	xmlNodePtr obsNode;
	xmlNodePtr propsNode;
	//xmlChar *szKey;          //��ʱ�ַ�������
	char *szDocName;
	CCMutableDictionary<std::string, CCString*>* propsTemp = new CCMutableDictionary<std::string, CCString*>();

	szDocName = "CreatedXml.xml";

	doc = xmlReadFile(szDocName,"UTF-8",XML_PARSE_RECOVER); //�����ļ�
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
		if (!xmlStrcmp(curNode->name, (const xmlChar *)"map"))
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
		}
		else
		{
			
			propsTemp->setObject(new CCString((const char*)xmlNodeGetContent(curNode)), string((char*)curNode->name));

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
	width = propsTemp->objectForKey("width")->toInt();
	height = propsTemp->objectForKey("height")->toInt();
	tileWidth = propsTemp->objectForKey("tilewidth")->toInt();
	tileHeight = propsTemp->objectForKey("tileheight")->toInt();
	background = propsTemp->objectForKey("background")->toInt();

	return true;
}
