/* token_VDM_prologue */
#if defined(__INTEL_COMPILER) && defined(_MSC_VER) && !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP __pragma(ivdep) __pragma(swp) __pragma(unroll) __pragma(prefetch)
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#elif !defined(VDM_MACRO_PRAGMA_IVDEP)
#   define VDM_MACRO_PRAGMA_IVDEP /* NOP */
#   define VDM_MACRO_PRAGMA_NO_IVDEP /* NOP */
#endif

// Scintilla source code edit control
/** @file SVector.h
 ** A simple expandable vector.
 **/
// Copyright 1998-2001 by Neil Hodgson <neilh@hare.net.au>
// The License.txt file describes the conditions under which this software may be distributed.

#ifndef SVECTOR_H
#define SVECTOR_H

#ifdef SCI_NAMESPACE
namespace Scintilla {
#endif

/**
 * A simple expandable integer vector.
 * Storage not allocated for elements until an element is used.
 * This makes it very lightweight unless used so is a good match for optional features.
 */
class SVector {
	enum { allocSize = 4000 };

	int *v;				///< The vector
	unsigned int size;	///< Number of elements allocated
	unsigned int len;	///< Number of elements used in vector

	/** Internally allocate more elements than the user wants
	 * to avoid thrashing the memory allocator. */
	void SizeTo(int newSize) {
		if (newSize < allocSize)
			newSize += allocSize;
		else
			newSize = (newSize * 3) / 2;
		int *newv = new int[newSize];
		size = newSize;
        unsigned int i=0;
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   pragma prefetch
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
		for (; i<len; i++) {
			newv[i] = v[i];
		}
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   pragma prefetch
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
		for (; i<size; i++) {
			newv[i] = 0;
		}
		delete []v;
		v = newv;
	}

public:
	SVector() {
		v = 0;
		len = 0;
		size = 0;
	}
	~SVector() {
		Free();
	}
	/// Constructor from another vector.
	SVector(const SVector &other) {
		v = 0;
		len = 0;
		size = 0;
		if (other.Length() > 0) {
			SizeTo(other.Length());
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   pragma prefetch
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
			for (int i=0; i<other.Length(); i++)
				v[i] = other.v[i];
			len = other.Length();
		}
	}
	/// Copy constructor.
	SVector &operator=(const SVector &other) {
		if (this != &other) {
			delete []v;
			v = 0;
			len = 0;
			size = 0;
			if (other.Length() > 0) {
				SizeTo(other.Length());
#if defined(__INTEL_COMPILER) && 1 // VDM auto patch
#   pragma ivdep
#   pragma swp
#   pragma unroll
#endif
#if defined(__INTEL_COMPILER) && 1 /* VDM auto patch */
#   pragma ivdep
#   pragma swp
#   pragma unroll
#   pragma prefetch
#   if 0
#       pragma simd noassert
#   endif
#endif /* VDM auto patch */
				for (int i=0; i<other.Length(); i++)
					v[i] = other.v[i];
				len = other.Length();
			}
		}
		return *this;
	}
	/** @brief Accessor.
	 * Allows to access values from the list, and grows it if accessing
	 * outside the current bounds. The returned value in this case is 0. */
	int &operator[](unsigned int i) {
		if (i >= len) {
			if (i >= size) {
				SizeTo(i);
			}
			len = i+1;
		}
		return v[i];
	}
	/// Reset vector.
	void Free() {
		delete []v;
		v = 0;
		size = 0;
		len = 0;
	}
	/** @brief Grow vector size.
	 * Doesn't allow a vector to be shrinked. */
	void SetLength(unsigned int newLength) {
		if (newLength > len) {
			if (newLength >= size) {
				SizeTo(newLength);
			}
		}
		len = newLength;
	}
	/// Get the current length (number of used elements) of the vector.
	int Length() const {
		return len;
	}
};

#ifdef SCI_NAMESPACE
}
#endif

#endif
