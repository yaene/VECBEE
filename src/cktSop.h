#ifndef CKT_SOP_H
#define CKT_SOP_H


#include <bits/stdc++.h>
#include "abcApi.h"
#include "cktBit.h"
#include "cktSopNet.h"


class Ckt_Sing_Sel_Info_t;
class Ckt_Sop_Net_t;


enum class Ckt_Sop_Cat_t
{
    PI, PO, CONST0, CONST1, INTER
};


class Ckt_Sop_t
{
private:
    //
    // common properties
    //
    abc::Abc_Obj_t *                        pAbcObj;            // the corresponding ABC object
    Ckt_Sop_Net_t *                         pCktNtk;            // the corresponding CKT network
    Ckt_Sop_Cat_t                           type;               // object type
    bool                                    isVisited;          // whether the object is visited
    int                                     topoId;             // mark the index in the topological sequence
    int                                     nLiterals;          // # literals
    std::vector <std::string>               SOP;                // positional cube notation
    std::vector <uint64_t>                  valueClusters;      // simluation value clusters
    std::vector <uint64_t>                  foConeInfo;         // mark whether POs are in the objects' fanout cone, each bit corresponds a PO
    std::vector <Ckt_Sop_t *>               pCktFanins;         // fanin pointers
    std::vector <Ckt_Sop_t *>               pCktFanouts;        // fanout pointers

    //
    // only used for cut network
    //
    Ckt_Sop_Net_t *                         pCktCutNtk;         // cut network, containing a copy of objects between itself and its cut
    Ckt_Sop_t *                             pCktObjOri;         // the original object in the parent network
    Ckt_Sop_t *                             pCktObjCopy;        // temporarily record the copied object

    Ckt_Sop_t &                             operator =          (const Ckt_Sop_t &);

public:
    explicit                                Ckt_Sop_t           (abc::Abc_Obj_t * p_abc_obj, Ckt_Sop_Net_t * p_ckt_ntk);
                                            Ckt_Sop_t           (const Ckt_Sop_t & other);
                                            ~Ckt_Sop_t          (void);
    void                                    PrintFanios         (void) const;
    void                                    CollectSOP          (void);
    int                                     GetLiteralsNum      (void);
    void                                    PrintClusters       (void) const;
    void                                    UpdateClusters      (void);
    void                                    UpdateCluster       (int i);
    void                                    FlipClustersFrom    (Ckt_Sop_t * pCktObj);
    uint64_t                                GetClusterValue     (std::vector <std::string> & newSOP, Ckt_Sop_Cat_t type, int i);
    void                                    ReplaceBy           (std::vector <std::string> & newSOP, Ckt_Sop_Cat_t _type, Ckt_Sing_Sel_Info_t & info);
    void                                    CheckFanio          (void) const;

    inline abc::Abc_Obj_t *                 GetAbcObj           (void) const                            { return pAbcObj; }
    inline Ckt_Sop_Net_t *                  GetCktNtk           (void) const                            { return pCktNtk; }
    inline Ckt_Sop_Cat_t                    GetType             (void) const                            { return type; }
    inline void                             SetType             (Ckt_Sop_Cat_t _type)                   { type = _type; }
    inline bool                             GetVisited          (void) const                            { return isVisited; }
    inline void                             SetVisited          (void)                                  { isVisited = true; }
    inline void                             ResetVisited        (void)                                  { isVisited = false; topoId = 0; }
    inline void                             ResetVisited2       (void)                                  { isVisited = false; }
    inline void                             SetTopoId           (int i)                                 { topoId = i; }
    inline int                              GetTopoId           (void) const                            { return topoId; }
    inline int                              GetNLiterals        (void) const                            { return nLiterals; }
    inline int                              GetClustersSize     (void) const                            { return static_cast <int> (valueClusters.size()); }
    inline void                             ResizeClusters      (int len)                               { valueClusters.resize(len); }
    inline void                             SetCluster          (int i, uint64_t value)                 { valueClusters[i] = value; }
    inline void                             FlipCluster         (int i)                                 { valueClusters[i] = ~valueClusters[i]; }
    inline uint64_t                         GetCluster          (int i) const                           { return valueClusters[i]; }
    inline int                              GetSOPSize          (void) const                            { return static_cast <int> (SOP.size()); }
    inline int                              GetSOPISize         (int i) const                           { return static_cast <int> (SOP[i].size()); }
    inline void                             SetSOP              (const std::vector <std::string> & s)   { SOP.assign(s.begin(), s.end()); }
    inline void                             SetSOPIJ            (int i, int j, char val)                { SOP[i][j] = val; }
    inline char                             GetSOPIJ            (int i, int j) const                    { return SOP[i][j]; }
    inline void                             CopySOP             (std::vector <std::string> & s) const   { s.assign(SOP.begin(), SOP.end()); }
    inline std::vector <std::string>        GetSOP              (void) const                            { return SOP; }
    inline void                             InitFoCone          (int f)                                 { foConeInfo.resize((f >> 6) + 1, 0); }
    inline int                              GetFoConeSize       (void) const                            { return static_cast <int> (foConeInfo.size()); }
    inline uint64_t                         GetFoCone           (int i) const                           { return foConeInfo[i]; }
    inline void                             SetFoCone           (int f)                                 { Ckt_SetBit(foConeInfo[f >> 6], f); }
    inline void                             SelfOrFoCone        (Ckt_Sop_t * pCktObj)                   { for (int i = 0; i < GetFoConeSize(); ++i) foConeInfo[i] |= pCktObj->foConeInfo[i]; }
    inline void                             AddFanio            (Ckt_Sop_t * pCktFanin)                 { pCktFanins.emplace_back(pCktFanin); pCktFanin->pCktFanouts.emplace_back(this); }
    inline void                             AddFanin            (Ckt_Sop_t * pCktFanin)                 { pCktFanins.emplace_back(pCktFanin); }
    inline Ckt_Sop_t *                      GetFanin            (int i = 0) const                       { return pCktFanins[i]; }
    inline int                              GetFaninNum         (void) const                            { return static_cast <int> (pCktFanins.size()); }
    inline void                             WriteFanin          (int i, Ckt_Sop_t * pCktFanin)          { pCktFanins[i] = pCktFanin; }
    inline Ckt_Sop_t *                      GetFanout           (int i = 0) const                       { return pCktFanouts[i]; }
    inline int                              GetFanoutNum        (void) const                            { return static_cast <int> (pCktFanouts.size()); }
    inline void                             InsertFanout        (int i, Ckt_Sop_t * pCktFanout)         { pCktFanouts.insert(pCktFanouts.begin() + i, pCktFanout); }
    inline void                             PopBackFanout       (void)                                  { pCktFanouts.pop_back(); }
    inline std::string                      GetName             (void) const                            { return std::string(Abc_ObjName(pAbcObj)); }
    inline bool                             IsPI                (void) const                            { return type == Ckt_Sop_Cat_t::PI; }
    inline bool                             IsPO                (void) const                            { return type == Ckt_Sop_Cat_t::PO; }
    inline bool                             IsConst             (void) const                            { return type == Ckt_Sop_Cat_t::CONST0 || type == Ckt_Sop_Cat_t::CONST1; }
    inline bool                             IsConst0            (void) const                            { return type == Ckt_Sop_Cat_t::CONST0; }
    inline bool                             IsConst1            (void) const                            { return type == Ckt_Sop_Cat_t::CONST1; }
    inline bool                             IsDanggling         (void) const                            { return pCktFanouts.empty() && !IsPO(); }
    inline void                             SetCutNtk           (Ckt_Sop_Net_t * p_ckt_cut_ntk)         { pCktCutNtk = p_ckt_cut_ntk; }
    inline Ckt_Sop_Net_t *                  GetCutNtk           (void) const                            { return pCktCutNtk; }
    inline void                             SetOriObj           (Ckt_Sop_t * pCktObj)                   { pCktObjOri = pCktObj; }
    inline Ckt_Sop_t *                      GetOriObj           (void) const                            { return pCktObjOri; }
    inline void                             SetCopiedObj        (Ckt_Sop_t * pCktObj)                   { pCktObjCopy = pCktObj; }
    inline Ckt_Sop_t *                      GetCopiedObj        (void) const                            { return pCktObjCopy; }
};


class Ckt_Sing_Sel_Info_t
{
public:
    Ckt_Sop_t *                             pCktObj;
    Ckt_Sop_Cat_t                           type;
    std::vector <std::string>               SOP;

    explicit                                Ckt_Sing_Sel_Info_t (void);
    explicit                                Ckt_Sing_Sel_Info_t (Ckt_Sop_t * p_ckt_obj, Ckt_Sop_Cat_t _type, const std::vector <std::string> & _sop);
                                            ~Ckt_Sing_Sel_Info_t(void);
};


std::ostream &                              operator <<         (std::ostream & os, const std::vector <std::string> & SOP);
std::ostream &                              operator <<         (std::ostream & os, const Ckt_Sop_Cat_t & type);
Ckt_Sop_Cat_t                               Abc_GetSopType      (abc::Abc_Obj_t * pObj);


#endif
