#ifndef __CFA_hpp__
#define __CFA_hpp__

// 2D - Name defined by 2 x 2 block order

// RGGB
// RGRGRGRGRGRGRG
// GBGBGBGBGBGBGB
// RGRGRGRGRGRGRG
// GBGBGBGBGBGBGB

// GRBG
// GRGRGRGRGRGRGR
// BGBGBGBGBGBGBG
// GRGRGRGRGRGRGR
// BGBGBGBGBGBGBG

// BGGR
// BGBGBGBGBGBGBG
// GRGRGRGRGRGRGR
// BGBGBGBGBGBGBG
// GRGRGRGRGRGRGR

// GBRG
// GBGBGBGBGBGBGB
// RGRGRGRGRGRGRG
// GBGBGBGBGBGBGB
// RGRGRGRGRGRGRG

// RGB - 3D
// Canal 0
// RRRRRRRRRRRRRR
// RRRRRRRRRRRRRR
// RRRRRRRRRRRRRR
// RRRRRRRRRRRRRR
// Canal 1
// GGGGGGGGGGGGGG
// GGGGGGGGGGGGGG
// GGGGGGGGGGGGGG
// GGGGGGGGGGGGGG
// Canal 2
// BBBBBBBBBBBBBB
// BBBBBBBBBBBBBB
// BBBBBBBBBBBBBB
// BBBBBBBBBBBBBB

enum CFA {
    RGGB = 0,
    GRBG,
    BGGR,
    GBRG,
    NONE
};

#endif