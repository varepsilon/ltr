// Copyright 2012 Yandex

#ifndef LTR_UTILITY_SHARED_PTR_H_
#define LTR_UTILITY_SHARED_PTR_H_

#include <algorithm>

#include <boost/shared_ptr.hpp> // NOLINT

using namespace boost; // NOLINT

namespace ltr {
namespace utility {

template<class T>
class shared_ptr {
 private:
  typedef shared_ptr<T> this_type;

 public:
  typedef T element_type;
  typedef T value_type;
  typedef T* pointer;
  typedef typename boost::detail::shared_ptr_traits<T>::reference reference;

  shared_ptr():
    px(0),
    pn() {
  }

  template<class Y>
  shared_ptr(Y* p): // NOLINT
    px(p),
    pn(p) {
    boost::detail::sp_enable_shared_from_this(this, p, p);
  }

  template<class Y, class D>
  shared_ptr(Y* p, D d):
    px(p),
    pn(p, d) {
    boost::detail::sp_enable_shared_from_this(this, p, p);
  }

  template<class Y, class D, class A>
  shared_ptr(Y* p, D d, A a):
    px(p),
    pn(p, d, a) {
    boost::detail::sp_enable_shared_from_this(this, p, p);
  }

  // generated copy constructor, destructor are fine
  template<class Y>
  shared_ptr(weak_ptr<Y> const &r): // NOLINT
    pn(r.pn) {
    px = r.px;
  }

  template<class Y>
  shared_ptr(weak_ptr<Y> const &r, boost::detail::sp_nothrow_tag):
    px(0),
    pn(r.pn, boost::detail::sp_nothrow_tag()) {
    if (!pn.empty()) {
        px = r.px;
    }
  }

  template<class Y>
#if !defined(BOOST_SP_NO_SP_CONVERTIBLE)
  shared_ptr(shared_ptr<Y> const &r,
             typename boost::detail::sp_enable_if_convertible<Y, T>::type =
             boost::detail::sp_empty())
#else
  shared_ptr(shared_ptr<Y> const &r) // NOLINT
#endif
  : px(r.px),
    pn(r.pn) {
  }

  // aliasing
  template<class Y>
  shared_ptr(shared_ptr<Y> const &r, T* p):
    px(p),
    pn(r.pn) {
  }

  template<class Y>
  shared_ptr(shared_ptr<Y> const &r,
             boost::detail::static_cast_tag):
    px(static_cast<element_type*>(r.px)),
    pn(r.pn) {
  }

  template<class Y>
  shared_ptr(shared_ptr<Y> const &r,
             boost::detail::const_cast_tag):
    px(const_cast<element_type *>(r.px)),
    pn(r.pn) {
  }

  template<class Y>
  shared_ptr(shared_ptr<Y> const &r,
             boost::detail::dynamic_cast_tag):
    px(dynamic_cast<element_type *>(r.px)),
    pn(r.pn) {
    if (px == 0) {
      pn = boost::detail::shared_count();
    }
  }

  template<class Y>
  shared_ptr(shared_ptr<Y> const &r,
             boost::detail::polymorphic_cast_tag):
    px(dynamic_cast<element_type *>(r.px)),
    pn(r.pn) {
    if (px == 0) {
      boost::throw_exception(std::bad_cast());
    }
  }

#ifndef BOOST_NO_AUTO_PTR
  template<class Y>
  shared_ptr(std::auto_ptr<Y> &r): // NOLINT
    px(r.get()),
    pn() {
    Y* tmp = r.get();
    pn = boost::detail::shared_count(r);
    boost::detail::sp_enable_shared_from_this(this, tmp, tmp);
  }

#if !defined(BOOST_NO_SFINAE) && \
  !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
  template<class Ap>
  shared_ptr(Ap r,
             typename boost::detail::sp_enable_if_auto_ptr<Ap, int>::type = 0):
    px(r.get()),
    pn() {
    typename Ap::element_type* tmp = r.get();
    pn = boost::detail::shared_count(r);
    boost::detail::sp_enable_shared_from_this(this, tmp, tmp);
  }


#endif  // BOOST_NO_SFINAE, BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif  // BOOST_NO_AUTO_PTR

  shared_ptr& operator = (shared_ptr const& r) {
    this_type(r).swap(*this);
    return *this;
  }

#if !defined(BOOST_MSVC) || (BOOST_MSVC >= 1400)
  template<class Y>
  shared_ptr& operator = (shared_ptr<Y> const& r) {
    this_type(r).swap(*this);
    return *this;
  }

#endif

#ifndef BOOST_NO_AUTO_PTR
  template<class Y>
  shared_ptr& operator = (std::auto_ptr<Y>& r) { // NOLINT
    this_type(r).swap(*this);
    return *this;
  }

#if !defined( BOOST_NO_SFINAE ) && \
  !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

  template<class Ap>
  typename boost::detail::sp_enable_if_auto_ptr<Ap, shared_ptr&>::type
    operator=(Ap r) {
    this_type(r).swap(*this);
    return *this;
  }

#endif  // BOOST_NO_SFINAE, BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#endif  // BOOST_NO_AUTO_PTR

// Move support

#if defined(BOOST_HAS_RVALUE_REFS)

  shared_ptr(shared_ptr&& r):
    px(r.px),
    pn() {
    pn.swap(r.pn);
    r.px = 0;
  }

  template<class Y>
#if !defined( BOOST_SP_NO_SP_CONVERTIBLE )
  shared_ptr(shared_ptr<Y>&& r,
    typename boost::detail::sp_enable_if_convertible<Y, T>::type =
    boost::detail::sp_empty())
#else
  shared_ptr(shared_ptr<Y>&& r)
#endif
  : px(r.px),
    pn() {
    pn.swap(r.pn);
    r.px = 0;
  }

  shared_ptr& operator = (shared_ptr&& r) {
    this_type(static_cast<shared_ptr&&>(r)).swap(*this);
    return *this;
  }

  template<class Y>
  shared_ptr& operator = (shared_ptr<Y>&& r) {
    this_type(static_cast<shared_ptr<Y>&& >(r)).swap(*this);
    return *this;
  }
#endif

  void reset() {
    this_type().swap(*this);
  }

  template<class Y>
  void reset(Y* p) {
    BOOST_ASSERT(p == 0 || p != px);  // catch self-reset errors
    this_type(p).swap(*this);
  }

  template<class Y, class D>
  void reset(Y* p, D d) {
    this_type(p, d).swap(*this);
  }

  template<class Y, class D, class A>
  void reset(Y* p, D d, A a) {
    this_type(p, d, a).swap(*this);
  }

  template<class Y>
  void reset(shared_ptr<Y> const& r, T* p) {
    this_type(r, p).swap(*this);
  }

  reference operator* () const {
    BOOST_ASSERT(px != 0);
    return *px;
  }

  T* operator -> () const {
    BOOST_ASSERT(px != 0);
    return px;
  }

  T* get() const {
    return px;
  }

// implicit conversion to "bool"
#include <boost/smart_ptr/detail/operator_bool.hpp> // NOLINT

  bool unique() const {
    return pn.unique();
  }

  long use_count() const { // NOLINT
    return pn.use_count();
  }

  void swap(shared_ptr<T> & other) {
    std::swap(px, other.px);
    pn.swap(other.pn);
  }

  template<class Y> bool
  _internal_less(shared_ptr<Y> const& rhs) const {
    return pn < rhs.pn;
  }

  void* _internal_get_deleter(boost::detail::sp_typeinfo const& ti) const {
    return pn.get_deleter( ti );
  }

  bool _internal_equiv(shared_ptr const & r) const {
    return px == r.px && pn == r.pn;
  }

// Tasteless as this may seem, making all members public allows member templates
// to work in the absence of member template friends. (Matthew Langston)

#ifndef BOOST_NO_MEMBER_TEMPLATE_FRIENDS

 private:
  template<class Y> friend class shared_ptr;
  template<class Y> friend class weak_ptr;
#endif

  T* px;                     // contained pointer
  boost::detail::shared_count pn;    // reference counter
};  // shared_ptr

template<class T, class U>
inline bool operator == (shared_ptr<T> const &a,
                         shared_ptr<U> const &b) {
  return a.get() == b.get();
}

template<class T, class U>
inline bool operator != (shared_ptr<T> const &a,
                         shared_ptr<U> const &b) {
  return a.get() != b.get();
}

#if __GNUC__ == 2 && __GNUC_MINOR__ <= 96

// Resolve the ambiguity between our op!= and the one in rel_ops

template<class T>
inline bool operator != (shared_ptr<T> const &a,
                         shared_ptr<T> const &b) {
  return a.get() != b.get();
}

#endif

template<class T, class U>
inline bool operator < (shared_ptr<T> const &a,
                        shared_ptr<U> const &b) {
  return a._internal_less(b);
}

template<class T>
inline void swap(shared_ptr<T>& a,
                 shared_ptr<T>& b) {
  a.swap(b);
}

template<class T, class U>
shared_ptr<T> static_pointer_cast(shared_ptr<U> const &r) {
  return shared_ptr<T>(r, boost::detail::static_cast_tag());
}

template<class T, class U>
shared_ptr<T> const_pointer_cast(shared_ptr<U> const &r) {
  return shared_ptr<T>(r, boost::detail::const_cast_tag());
}

template<class T, class U>
shared_ptr<T> dynamic_pointer_cast(shared_ptr<U> const &r) {
  return shared_ptr<T>(r, boost::detail::dynamic_cast_tag());
}

// shared_*_cast names are deprecated. Use *_pointer_cast instead.

template<class T, class U>
shared_ptr<T> shared_static_cast(shared_ptr<U> const &r) {
  return shared_ptr<T>(r, boost::detail::static_cast_tag());
}

template<class T, class U>
shared_ptr<T> shared_dynamic_cast(shared_ptr<U> const &r) {
  return shared_ptr<T>(r, boost::detail::dynamic_cast_tag());
}

template<class T, class U>
shared_ptr<T> shared_polymorphic_cast(shared_ptr<U> const &r) {
  return shared_ptr<T>(r, boost::detail::polymorphic_cast_tag());
}

template<class T, class U>
shared_ptr<T> shared_polymorphic_downcast(shared_ptr<U> const &r) {
  BOOST_ASSERT(dynamic_cast<T *>(r.get()) == r.get());
  return shared_static_cast<T>(r);
}

// get_pointer() enables boost::mem_fn to recognize shared_ptr

template<class T>
inline T* get_pointer(shared_ptr<T> const &p) {
  return p.get();
}

#if !defined(BOOST_NO_IOSTREAM)

#if defined(BOOST_NO_TEMPLATED_IOSTREAMS) || \
  (defined(__GNUC__) && (__GNUC__ < 3))
template<class Y>
std::ostream& operator << (std::ostream& os, shared_ptr<Y> const &p) {
  os << p.get();
  return os;
}
#else
// in STLport's no-iostreams mode no iostream symbols can be used
#ifndef _STLP_NO_IOSTREAMS
#if defined(BOOST_MSVC) && \
  BOOST_WORKAROUND(BOOST_MSVC, < 1300 && __SGI_STL_PORT)
// MSVC6 has problems finding std::basic_ostream through the
// using declaration in namespace _STL
using std::basic_ostream;
template<class E, class T, class Y>
basic_ostream<E, T>& operator << (
  basic_ostream<E, T>& os, shared_ptr<Y> const& p)
#else
template<class E, class T, class Y>
std::basic_ostream<E, T>& operator << (std::basic_ostream<E, T>& os, // NOLINT
                                       shared_ptr<Y> const &p)
#endif
{ // NOLINT
  os << p.get();
  return os;
}
#endif  // _STLP_NO_IOSTREAMS

#endif  // __GNUC__ < 3

#endif  // !defined(BOOST_NO_IOSTREAM)

// get_deleter

#if (defined(__GNUC__) && BOOST_WORKAROUND(__GNUC__, < 3)) || \
    (defined(__EDG_VERSION__) && BOOST_WORKAROUND(__EDG_VERSION__, <= 238)) || \
    (defined(__HP_aCC) && BOOST_WORKAROUND(__HP_aCC, <= 33500))

// g++ 2.9x doesn't allow static_cast<X const *>(void *)
// apparently EDG 2.38 and HP aCC A.03.35 also don't accept it

template<class D, class T>
D* get_deleter(shared_ptr<T> const &p) {
  void const * q = p._internal_get_deleter(BOOST_SP_TYPEID(D));
  return const_cast<D *>(static_cast<D const *>(q));
}

#else

template<class D, class T>
D* get_deleter(shared_ptr<T> const &p) {
  return static_cast<D *>(p._internal_get_deleter(BOOST_SP_TYPEID(D)));
}

#endif

// atomic access

#if !defined(BOOST_SP_NO_ATOMIC_ACCESS)

template<class T>
inline bool atomic_is_lock_free(shared_ptr<T> const* /*p*/) {
  return false;
}

template<class T>
shared_ptr<T> atomic_load(shared_ptr<T> const* p) {
  boost::detail::spinlock_pool<2>::scoped_lock lock(p);
  return *p;
}

template<class T>
inline shared_ptr<T> atomic_load_explicit(shared_ptr<T> const* p,
                                              memory_order /*mo*/ ) {
  return atomic_load( p );
}

template<class T>
void atomic_store(shared_ptr<T>* p, shared_ptr<T> r) {
  boost::detail::spinlock_pool<2>::scoped_lock lock(p);
  p->swap(r);
}

template<class T>
inline void atomic_store_explicit(shared_ptr<T>* p,
                                  shared_ptr<T> r,
                                  memory_order /*mo*/) {
  atomic_store(p, r);  // std::move( r )
}

template<class T>
shared_ptr<T> atomic_exchange(shared_ptr<T>* p,
                                  shared_ptr<T> r) {
  boost::detail::spinlock& sp =
    boost::detail::spinlock_pool<2>::spinlock_for(p);
  sp.lock();
  p->swap(r);
  sp.unlock();
  return r;  // return std::move( r )
}

template<class T>
shared_ptr<T> atomic_exchange_explicit(shared_ptr<T>* p,
                                            shared_ptr<T> r,
                                            memory_order /*mo*/) {
  return atomic_exchange(p, r);  // std::move(r)
}

template<class T>
bool atomic_compare_exchange(shared_ptr<T>* p,
                              shared_ptr<T>* v,
                              shared_ptr<T> w) {
  boost::detail::spinlock & sp =
    boost::detail::spinlock_pool<2>::spinlock_for(p);
  sp.lock();

  if (p->_internal_equiv(*v)) {
    p->swap(w);
    sp.unlock();
    return true;
  } else {
    shared_ptr<T> tmp(*p);
    sp.unlock();
    tmp.swap(*v);
    return false;
  }
}

template<class T>
inline bool atomic_compare_exchange_explicit(shared_ptr<T>* p,
                                              shared_ptr<T>* v,
                                              shared_ptr<T> w,
                                              memory_order /*success*/,
                                              memory_order /*failure*/) {
  return atomic_compare_exchange( p, v, w );  // std::move( w )
}

#endif  // !defined(BOOST_SP_NO_ATOMIC_ACCESS)

// hash_value

template<class T>
struct hash;

template<class T>
std::size_t hash_value(shared_ptr<T> const &p) {
  return boost::hash<T*>()(p.get());
}
};
};
#endif  // LTR_UTILITY_SHARED_PTR_H_
