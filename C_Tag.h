#pragma once
#ifndef C_Tag_h
#define C_Tag_h

#include "Component.h"

enum class Tag
{
    Default,
    Player,
    NPC,
    Enemy
};

class C_Tag : public Component
{
public:
    C_Tag(Object* owner) : Component(owner), tag(Tag::Default)
    {

    };
    virtual ~C_Tag() = default;

    Tag Get() const;
    void Set(Tag tag);

    bool Compare(std::shared_ptr<C_Tag> other) const;
    bool Compare(Tag other) const;

private:
    Tag tag;
};


#endif /* C_Tag_h */