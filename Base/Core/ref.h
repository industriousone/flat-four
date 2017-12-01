/**
 * @file ref.h
 *
 * @author  Jason Perkins (jason@379.com)
 * @date    Copyright (C) 2001 by 379, Inc.
 * @version $Id: ref.h,v 1.2 2002/04/22 01:51:55 jason379 Exp $
 */
#if !defined(_F4_REF_H_)
#define _F4_REF_H_

ENTER_NAMESPACE(f4);


/**
 * A template based smart-pointer to handle the reference counting of
 * service instances.
 * @ingroup f4_core
 */
template<class T>
class Ref
{
public:
	Ref() : m_ptr(0)                       { }
	Ref(T* ptr) : m_ptr(ptr)               { if (m_ptr) m_ptr->reference(); }
	Ref(const Ref& ref) : m_ptr(ref.m_ptr) { if (m_ptr) m_ptr->reference(); }
	
	~Ref() { if (m_ptr) m_ptr->release(); }

	T* ptr()                     { return m_ptr; }
	T* operator ->()             { return m_ptr; }
	const T* operator ->() const { return m_ptr; }
	operator T*()                { return m_ptr; }
	operator const T*() const    { return m_ptr; }

	Ref& operator =(const Ref& ref)
	{
		if (ref.m_ptr) ref.m_ptr->reference();
		if (m_ptr) m_ptr->release();
		m_ptr = ref.m_ptr;
		return *this;
	}

	Ref& operator =(T* ptr)
	{
		if (ptr) ptr->reference();
		if (m_ptr) m_ptr->release();
		m_ptr = ptr;
		return *this;
	}

	bool operator ==(const Ref& ref) const
	{
		return m_ptr == ref.m_ptr;
	}

	bool operator ==(const T* ptr) const
	{
		return m_ptr == ptr;
	}

	bool operator !=(const Ref& ref) const
	{
		return (m_ptr != ref.m_ptr);
	}

	bool operator !=(const T* ptr) const
	{
		return (m_ptr != ptr);
	}


private:
	T* m_ptr;
};


EXIT_NAMESPACE;
#endif
