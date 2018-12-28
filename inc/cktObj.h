#ifndef CKT_OBJ_H
#define CKT_OBJ_H


#include <bits/stdc++.h>
#include "abcApi.h"


enum class Ckt_Obj_Type_t
{
    PI,   PO,   CONST0, CONST1, BUF,   INV,   XOR,
    XNOR, AND2, OR2,    NAND2,  NAND3, NAND4, NOR2,
    NOR3, NOR4, AOI21,  AOI22,  OAI21, OAI22
};


class Ckt_Rpl_Info_t;


class Ckt_Obj_t
{
private:
    abc::Abc_Obj_t *          pAbcObj;          // the corresponding ABC object
    Ckt_Obj_Type_t            type;             // object type
    bool                      isVisited;        // whether the object is visited
    std::vector <uint64_t>    valueClusters;    // simluation value clusters
    std::vector <Ckt_Obj_t *> pCktFanins;       // fanin pointers
    std::vector <Ckt_Obj_t *> pCktFanouts;      // fanout pointers

    Ckt_Obj_t &               operator =        (const Ckt_Obj_t & other);

public:
    explicit                  Ckt_Obj_t         (abc::Abc_Obj_t * p_abc_obj);
                              Ckt_Obj_t         (const Ckt_Obj_t & other);
                              ~Ckt_Obj_t        (void);
    void                      PrintFanios       (void) const;
    void                      PrintClusters     (void) const;
    void                      UpdateClusters    (void);
    void                      ReplaceBy         (Ckt_Obj_t & cktNewObj, std::vector <Ckt_Rpl_Info_t> & info);
    void                      CheckFanio        (void) const;

    inline abc::Abc_Obj_t *   GetAbcObj         (void) const                    { return pAbcObj; }
    inline Ckt_Obj_Type_t     GetType           (void) const                    { return type; }
    inline bool               GetVisited        (void) const                    { return isVisited; }
    inline void               SetVisited        (void)                          { isVisited = true; }
    inline void               ResetVisited      (void)                          { isVisited = false; }
    inline int                GetClustersSize   (void) const                    { return static_cast <int> (valueClusters.size()); }
    inline void               ResizeClusters    (int len)                       { valueClusters.resize(len); }
    inline void               SetCluster        (int i, uint64_t value)         { valueClusters[i] = value; }
    inline uint64_t           GetCluster        (int i) const                   { return valueClusters[i]; }
    inline void               AddFanin          (Ckt_Obj_t * pCktFanin)         { pCktFanins.emplace_back(pCktFanin); pCktFanin->pCktFanouts.emplace_back(this); }
    inline Ckt_Obj_t *        GetFanin          (int i = 0) const               { return pCktFanins[i]; }
    inline int                GetFaninNum       (void) const                    { return static_cast <int> (pCktFanins.size()); }
    inline void               WriteFanin        (int i, Ckt_Obj_t * pCktFanin)  { pCktFanins[i] = pCktFanin; }
    inline Ckt_Obj_t *        GetFanout         (int i = 0) const               { return pCktFanouts[i]; }
    inline int                GetFanoutNum      (void) const                    { return static_cast <int> (pCktFanouts.size()); }
    inline void               InsertFanout      (int i, Ckt_Obj_t * pCktFanout) { pCktFanouts.insert(pCktFanouts.begin() + i, pCktFanout); }
    inline void               PopBackFanout     (void)                          { pCktFanouts.pop_back(); }
    inline std::string        GetName           (void) const                    { return std::string(Abc_ObjName(pAbcObj)); }
    inline float              GetArrivalTime    (void) const                    { return (static_cast<abc::Abc_Time_t *>(pAbcObj->pNtk->pManTime->vArrs->pArray[pAbcObj->Id]))->Rise; }
    inline bool               IsPI              (void) const                    { return type == Ckt_Obj_Type_t::PI; }
    inline bool               IsPO              (void) const                    { return type == Ckt_Obj_Type_t::PO; }
    inline bool               IsConst           (void) const                    { return type == Ckt_Obj_Type_t::CONST0 || type == Ckt_Obj_Type_t:: CONST1; }
};


class Ckt_Rpl_Info_t
{
public:
    Ckt_Obj_t *               pCktObjFrom;
    int                       iCktFanin;
    Ckt_Obj_t *               pCktObjTo;
    int                       iCktFanout;

    explicit                  Ckt_Rpl_Info_t    (Ckt_Obj_t * pObj1, int iFanin, Ckt_Obj_t * pObj2, int iFanout);
                              ~Ckt_Rpl_Info_t   (void);
};


std::ostream &                operator <<       (std::ostream & os, const Ckt_Obj_Type_t & type);
std::ostream &                operator <<       (std::ostream & os, const Ckt_Rpl_Info_t & info);
Ckt_Obj_Type_t                Ckt_GetObjType    (abc::Abc_Obj_t * pAbcObj);
bool                          Ckt_SopIsConst0   (char * pSop);
bool                          Ckt_SopIsConst1   (char * pSop);
bool                          Ckt_SopIsBuf      (char * pSop);
bool                          Ckt_SopIsInvGate  (char * pSop);
bool                          Ckt_SopIsAndGate  (char * pSop);
bool                          Ckt_SopIsOrGate   (char * pSop);
bool                          Ckt_SopIsNandGate (char * pSop);
bool                          Ckt_SopIsNorGate  (char * pSop);
bool                          Ckt_SopIsXorGate  (char * pSop);
bool                          Ckt_SopIsXnorGate (char * pSop);
bool                          Ckt_SopIsAOI21Gate(char * pSop);
bool                          Ckt_SopIsAOI22Gate(char * pSop);
bool                          Ckt_SopIsOAI21Gate(char * pSop);
bool                          Ckt_SopIsOAI22Gate(char * pSop);

static inline void            Ckt_SetBit        (uint64_t & x, uint64_t f) { x |= ((uint64_t)1 << (f & (uint64_t)63)); }
static inline void            Ckt_ResetBit      (uint64_t & x, uint64_t f) { x &= ~((uint64_t)1 << (f & (uint64_t)63)); }
static inline bool            Ckt_GetBit        (uint64_t x, uint64_t f) { return (bool)((x >> f) & (uint64_t)1); }

#endif
