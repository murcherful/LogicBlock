#ifndef COMMON_H
#define COMMON_H

#include <unordered_map>
#include <unordered_set>
#define BLOCK_SIZE 32
#define WHELL_SCALE_SPEED 0.1
#define SCENE_WIDTH 200
#define SCENE_HEIGHT 200
#define KEY_MOVE_SPEED 5

typedef std::pair<int, int> IndexPair;

struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};



#endif // COMMON_H
