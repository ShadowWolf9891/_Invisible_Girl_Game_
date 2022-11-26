#include "XML_Parser.h"

XML_Parser::XML_Parser(SharedContext& context): context(context)
{

}

std::unordered_map<AnimationState, AnimationList> XML_Parser::LoadAnimFromFile(const std::string& filename)
{
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
			for (auto item = obj->FirstChildElement("Item"); item != nullptr; item = item->NextSiblingElement("Item")) 
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
