#include "Level.h"
#include "libxml/tree.h"

Level::Level(void)
{
}

Level::~Level(void)
{
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
	int nRel = xmlSaveFile("CreatedXml.xml",doc);
	if (nRel == -1)
	{
		return false;
	}

	//�ͷ��ĵ��ڽڵ㶯̬������ڴ�
	xmlFreeDoc(doc);

	return true;
}
