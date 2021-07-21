#ifndef LATEX_ATOM_SIDESET_H
#define LATEX_ATOM_SIDESET_H

#include "atom/atom.h"

namespace tex {

/** An atom representing scripts around a base atom */
class SideSetsAtom : public Atom {
public:
  sptr<Atom> _left, _right, _base;

  SideSetsAtom() = delete;

  SideSetsAtom(const sptr<Atom>& base, const sptr<Atom>& left, const sptr<Atom>& right)
    : _base(base), _left(left), _right(right) {
    _type = AtomType::bigOperator;
    _limitsType = LimitsType::noLimits;
  }

  sptr<Box> createBox(Env& env) override;

  __decl_clone(SideSetsAtom)
};

}

#endif //LATEX_ATOM_SIDESET_H
