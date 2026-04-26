#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "Observer.h"

class Observable {
public:
    void addObserver(Observer* observer) {
        if (observer == nullptr) {
            return;
        }
        if (std::find(observers_.begin(), observers_.end(), observer) == observers_.end()) {
            observers_.push_back(observer);
        }
    }

    void removeObserver(Observer* observer) {
        observers_.erase(std::remove(observers_.begin(), observers_.end(), observer), observers_.end());
    }

protected:
    void notify(const std::string& message) {
        for (Observer* observer : observers_) {
            if (observer != nullptr) {
                observer->update(message);
            }
        }
    }

private:
    std::vector<Observer*> observers_;
};