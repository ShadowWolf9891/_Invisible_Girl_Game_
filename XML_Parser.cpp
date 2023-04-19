#include "XML_Parser.h"

XML_Parser::XML_Parser(SharedContext& context): context(context)
{

}

std::unordered_map<AnimationState, AnimationList> XML_Parser::LoadAnimFromFile(const std::string& filename)
{
	animations.clear(); //Reset Animations for each loaded file
	opened_documents.emplace_back(new tinyxml2::XMLDocument());
	tinyxml2::XMLDocument* doc = opened_documents.back().get();

	doc->LoadFile(filename.c_str());
	ParseAnimDoc(doc);

	return this->animations;
}

std::unordered_map<std::string, ObjectData> XML_Parser::LoadInitialObjectDataFromFile(const std::string& filename)
{
	opened_documents.emplace_back(new tinyxml2::XMLDocument());
	tinyxml2::XMLDocument* doc = opened_documents.back().get();

	doc->LoadFile(filename.c_str());
	ParseInitDoc(doc);

	return this->objects;
}

std::unordered_map<int, std::unordered_map<StatusType, std::vector<std::shared_ptr<DialogueNode>>>> XML_Parser::LoadDialogueDataFromFile(const std::string& filename)
{
	opened_documents.emplace_back(new tinyxml2::XMLDocument());
	tinyxml2::XMLDocument* doc = opened_documents.back().get();

	doc->LoadFile(filename.c_str());
	ParseDialogueDoc(doc);
	return dNodes;
}

std::unordered_map<std::string, UIData> XML_Parser::LoadUIFromFile(const std::string& filename)
{
	opened_documents.emplace_back(new tinyxml2::XMLDocument());
	tinyxml2::XMLDocument* doc = opened_documents.back().get();

	doc->LoadFile(filename.c_str());

	ParseUIFormatDoc(doc);
	return uiObjects;
}




void XML_Parser::ParseAnimDoc(tinyxml2::XMLDocument* doc)
{
	if (doc->Error())
	{
		char buffer[200];
		sprintf(buffer, "Error parsing the XML: %s", doc->ErrorName());
		throw std::exception(buffer);
	}

	const tinyxml2::XMLElement* xml_root = doc->RootElement();
	
	if (xml_root->Attribute("_type", "animation")) //Is a XML file describing animation stuff
	{
		for (auto animGroupNode = xml_root->FirstChildElement("AnimationGroup"); //Recursivly get all animation groups
			animGroupNode != nullptr;
			animGroupNode = animGroupNode->NextSiblingElement("AnimationGroup"))
		{
			//Get attributes for AnimationGroup
			AnimationState state;
			std::shared_ptr <AnimationList> animList = std::make_shared<AnimationList>();
			std::string filename, looped;
			bool isLoop = false;
			int fileNum;
			if (animGroupNode->Attribute("state")) 
			{
				auto it = strToAnimState.find(animGroupNode->Attribute("state"));
				if (it != strToAnimState.end())
				{
					state = it->second;
				}
			}
			else 
			{
				char buffer[200];
				sprintf(buffer, "Error parsing the AnimationState from XML: %s", doc->ErrorName());
				throw std::exception(buffer);
			}
			if (animGroupNode->Attribute("looped"))
			{
				looped = animGroupNode->Attribute("looped");
				if (looped == "true") 
				{
					isLoop = true;
				}
				else 
				{
					isLoop = false;
				}
			}
			if (animGroupNode->Attribute("filename")) 
			{
				filename = animGroupNode->Attribute("filename");
				fileNum = context.textureAllocator->Add(context.workingDir->Get() + filename);
			}
			else
			{
				char buffer[200];
				sprintf(buffer, "Error parsing the Filename from XML: %s", doc->ErrorName());
				throw std::exception(buffer);
			}

			//Loop through Animations recursivly
			for (auto animNode = animGroupNode->FirstChildElement("Animation");
				animNode != nullptr;
				animNode = animNode->NextSiblingElement("Animation"))
			{
				FacingDirection dir;
				std::shared_ptr<Animation> anim = std::make_shared<Animation>();
				if (animNode->Attribute("direction")) {

					auto it = strToDir.find(animNode->Attribute("direction"));
					if (it != strToDir.end())
					{
						dir = it->second;
					}
				}
				else
				{
					char buffer[200];
					sprintf(buffer, "Error parsing the direction from XML: %s", doc->ErrorName());
					throw std::exception(buffer);
				}
				int frameWidth, frameHeight;
				if (animNode->Attribute("frameWidth") && animNode->Attribute("frameHeight"))
				{
					frameWidth = atoi(animNode->Attribute("frameWidth"));
					frameHeight = atoi(animNode->Attribute("frameHeight"));
				}
				else
				{
					char buffer[200];
					sprintf(buffer, "Error parsing the frame size from XML: %s", doc->ErrorName());
					throw std::exception(buffer);
				}
				for (auto frameNode = animNode->FirstChildElement("Frame"); 
					frameNode != nullptr;
					frameNode = frameNode->NextSiblingElement("Frame"))
				{

					int x, y;
					float duration;
					if (frameNode->Attribute("x") && frameNode->Attribute("y") && frameNode->Attribute("duration"))
					{
						
						x = atoi(frameNode->Attribute("x"));
						y = atoi(frameNode->Attribute("y"));
						duration = atof(frameNode->Attribute("duration"));
						
						
						
					}
					else
					{
						char buffer[200];
						sprintf(buffer, "Error parsing the frame from XML: %s", doc->ErrorName());
						throw std::exception(buffer);
					}
					
					anim->AddFrame(fileNum, x, y, frameWidth, frameHeight, duration);

				}
				anim->SetLooped(isLoop);
				animList->insert(std::make_pair(dir, anim));

			}

			animations.insert(std::make_pair(state, *animList));

		}
	}
}

void XML_Parser::ParseInitDoc(tinyxml2::XMLDocument* doc)
{
	objects.clear();

	if (doc->Error())
	{
		char buffer[200];
		sprintf(buffer, "Error parsing the XML: %s", doc->ErrorName());
		throw std::exception(buffer);
	}

	const tinyxml2::XMLElement* xml_root = doc->RootElement();

	if (xml_root->Attribute("_type", "initData")) //Is a XML file describing initial data stuff
	{
		for (auto obj = xml_root->FirstChildElement("Object"); //Recursivly get all objects
			obj != nullptr;
			obj = obj->NextSiblingElement("Object"))
		{
			ObjectData objData = ObjectData();
			std::string _name = "";
			Bitmask componentMask;

			if (obj->Attribute("name"))
			{
				_name = obj->Attribute("name");
			}
			auto componentsNode = obj->FirstChildElement("Components");

			if (componentsNode->Attribute("bitmap"))
			{
				componentMask = static_cast<uint32_t>(std::stoul(componentsNode->Attribute("bitmap"), nullptr, 2));
				objData.componentMask = componentMask;
			}
			
			auto propertiesNode = obj->FirstChildElement("Properties");
			for (auto prop = propertiesNode->FirstChildElement("Property"); //Recursivly get all objects
				prop != nullptr;
				prop = prop->NextSiblingElement("Property"))
			{
				std::string name, value;

				if (prop->Attribute("name") && prop->Attribute("value"))
				{
					name = prop->Attribute("name");
					value = prop->Attribute("value");

					objData.properties.insert(std::make_pair(name, value));
				}
				else 
				{
					char buffer[200];
					sprintf(buffer, "Error parsing property from XML: %s", doc->ErrorName());
					throw std::exception(buffer);
				}
			}
			for (auto item = obj->FirstChildElement("Icon"); item != nullptr; item = item->NextSiblingElement("Icon")) 
			{
				if (item->Attribute("ID"))objData.properties.at("ID") = item->Attribute("ID");
				
				if (item->Attribute("X")) objData.properties.at("X") = item->Attribute("X");
				
				if (item->Attribute("Y")) objData.properties.at("Y") = item->Attribute("Y");

				if (item->Attribute("name")) _name = item->Attribute("name");

				if (item->Attribute("description")) objData.properties.at("Description") = item->Attribute("description");

			}

			objects.insert(std::make_pair(_name, objData));
		}
	}
}

void XML_Parser::ParseDialogueDoc(tinyxml2::XMLDocument* doc)
{
	if (doc->Error())
	{
		char buffer[200];
		sprintf(buffer, "Error parsing the XML: %s", doc->ErrorName());
		throw std::exception(buffer);
	}

	const tinyxml2::XMLElement* xml_root = doc->RootElement();

	if (xml_root->Attribute("_type", "QuestDialogue")) //Is a XML file describing Dialogue stuff
	{
		for (auto questNode = xml_root->FirstChildElement("Quest"); //Recursivly get all Quests
			questNode != nullptr;
			questNode = questNode->NextSiblingElement("Quest"))
		{
			//Get attributes for questNode
			
			int questID;
			std::unordered_map<StatusType, std::vector < std::shared_ptr<DialogueNode>>> statusDMap;

			if (questNode->Attribute("id"))
			{
				questID = std::stoi(questNode->Attribute("id"));
			}
			else
			{
				char buffer[200];
				sprintf(buffer, "Error parsing the Quest id from XML: %s", doc->ErrorName());
				throw std::exception(buffer);
			}
			
			//Loop through quest statuses recursivly
			for (auto statusNode = questNode->FirstChildElement("QuestStatus");
				statusNode != nullptr;
				statusNode = statusNode->NextSiblingElement("QuestStatus"))
			{
				StatusType questStatus;
				std::vector<std::shared_ptr<DialogueNode>> dialogueNodes;

				if (statusNode->Attribute("status")) {

					auto it = strToQuestStatus.find(statusNode->Attribute("status"));
					if (it != strToQuestStatus.end())
					{
						questStatus = it->second;
					}
				}
				else
				{
					char buffer[200];
					sprintf(buffer, "Error parsing the questStatus from XML: %s", doc->ErrorName());
					throw std::exception(buffer);
				}

				for (auto dialogNode = statusNode->FirstChildElement("DialogueNode");
					dialogNode != nullptr;
					dialogNode = dialogNode->NextSiblingElement("DialogueNode"))
				{
					std::shared_ptr<DialogueNode> dNode = std::make_shared<DialogueNode>();
					
					if (dialogNode->Attribute("id"))
					{
						dNode->ID = atoi(dialogNode->Attribute("id"));
					}
					else
					{
						char buffer[200];
						sprintf(buffer, "Error parsing the dialog node id from XML: %s", doc->ErrorName());
						throw std::exception(buffer);
					}
					if (dialogNode->Attribute("text"))
					{
						dNode->text = dialogNode->Attribute("text");
					}
					else
					{
						char buffer[200];
						sprintf(buffer, "Error parsing the dialog node text from XML: %s", doc->ErrorName());
						throw std::exception(buffer);
					}

					for (auto optNode = dialogNode->FirstChildElement("DialogueOption");
						optNode != nullptr;
						optNode = optNode->NextSiblingElement("DialogueOption"))
					{
						std::shared_ptr<OptionNode> dOption = std::make_shared<OptionNode>();

						if (optNode->Attribute("nextNodeID"))
						{
							dOption->nextNodeID = atoi(optNode->Attribute("nextNodeID"));
						}
						else
						{
							dOption->nextNodeID = -1;
						}
						if (optNode->Attribute("text"))
						{
							dOption->text = optNode->Attribute("text");
						}
						else 
						{
							dOption->text = "";
						}
						if (optNode->Attribute("returnCode")) {

							auto it = strToQuestStatus.find(optNode->Attribute("returnCode"));
							if (it != strToQuestStatus.end())
							{
								dOption->returnCode = it->second;
							}
							else 
							{
								char buffer[200];
								sprintf(buffer, "Error parsing the returnCode from XML: %s", doc->ErrorName());
								throw std::exception(buffer);
							}
						}
						else
						{
							dOption->returnCode = questStatus;
						}

						dNode->options.emplace_back(dOption);
					}

					dialogueNodes.emplace_back(dNode);

				}
				
				statusDMap.emplace(questStatus, dialogueNodes);

			}
				dNodes.emplace(questID, statusDMap);
		}
	}

}

void XML_Parser::ParseUIFormatDoc(tinyxml2::XMLDocument* doc)
{
	if (doc->Error())
	{
		char buffer[200];
		sprintf(buffer, "Error parsing the XML: %s", doc->ErrorName());
		throw std::exception(buffer);
	}

	const tinyxml2::XMLElement* xml_root = doc->RootElement();

	if (xml_root->Attribute("_type", "UIFormat")) //Is a XML file describing animation stuff
	{
		for (auto UIElement = xml_root->FirstChildElement();
			UIElement != nullptr && UIElement != xml_root; ) //Recursivly get all UIElements
			
		{
			//Get attributes for UIElement

			UIData data = UIData();
			if (UIElement->Parent()->ToElement() == xml_root)
			{
				data.parentName = "root";
			}
			else
			{
				data.parentName = UIElement->Parent()->ToElement()->Attribute("Name");
			}
			
			data.uiType = UIElement->Value();

			for (auto a = UIElement->FirstAttribute(); a != nullptr; a = a->Next())
			{
				data.properties.insert(std::make_pair(a->Name(), a->Value()));
			}
			data.name = data.properties.at("Name");

			if (UIElement->NoChildren())
			{
				if (UIElement->NextSiblingElement())
				{
					uiObjects.insert(std::make_pair(data.name, data));
					UIElement = UIElement->NextSiblingElement();
				}
				else
				{
					uiObjects.insert(std::make_pair(data.name, data));
					UIElement = UIElement->Parent()->ToElement();
				}
			}
			else
			{
				if (uiObjects.find(data.name) == uiObjects.end())
				{
					for (auto child = UIElement->FirstChildElement(); child != nullptr; child = child->NextSiblingElement())
					{
						if (child->Attribute("Name"))
						{
							data.childrenNames.push_back(child->Attribute("Name"));
						}
					}

					uiObjects.insert(std::make_pair(data.name, data));

					UIElement = UIElement->FirstChildElement();
				}
				else
				{
					if (UIElement->NextSiblingElement())
					{
						UIElement = UIElement->NextSiblingElement();
					}
					else
					{
						if (UIElement->Parent()->ToElement()->Name() != "root")
						{
							UIElement = UIElement->Parent()->ToElement();
						}
						else
						{
							break;
						}
						
					}
				}
			}
			
			
		}
	}
}