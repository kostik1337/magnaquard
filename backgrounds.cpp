#include "backgrounds.h"
#include "world.h"

Backgrounds::Backgrounds(Context *context)
    : LogicComponent(context) {
}

void Backgrounds::OnNodeSet(Node *node) {
    if (node) {
        Node *backing = node->GetChild("Backing", true);
        for (int i=0; i<4; ++i) {
            Node* n = i == 0 ? backing : backing->Clone();
            n->SetPosition(Vector3(0, 0, (i - 2) * backingSize));
            backings.Push(n);
        }
    }
}

void Backgrounds::Update(float timeStep) {
    int speed = 5;
    for (Node* n: backings) {
        Vector3 newPos = n->GetPosition() - Vector3(0, 0, speed * timeStep);
        if(newPos.z_ < -backingSize * 2) newPos.z_ += backingSize * backings.Size();
        n->SetPosition(newPos);
    }

    for (auto it = bonuses.Begin(); it < bonuses.End();) {
        if (Node* n = (*it).Lock()) {
            Vector3 newPos = n->GetPosition() - Vector3(0, 0, speed * timeStep);
            if (newPos.z_ < -backingSize * 2) newPos.z_ += fieldRepeatSize;
            n->SetPosition(newPos);
            ++it;
        } else {
            it = bonuses.Erase(it);
        }
    }
}

void Backgrounds::CreateBonus(Vector3 position) {
    bonuses.Push(WeakPtr<Node>(GetSubsystem<World>()->SpawnBonus(position)));
}
