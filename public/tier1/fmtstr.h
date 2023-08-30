//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: A simple class for performing safe and in-expression sprintf-style
//			string formatting
//
// $NoKeywords: $
//=============================================================================//

#ifndef FMTSTR_H
#define FMTSTR_H

#include <stdarg.h>
#include <stdio.h>
#include "tier0/platform.h"
#include "tier0/dbg.h"
#include "tier1/strtools.h"

#if defined( _WIN32 )
#pragma once
#endif
#if defined(POSIX)
#pragma GCC visibility push(hidden)
#endif

//=============================================================================

// using macro to be compatable with GCC
#define FmtStrVSNPrintf( szBuf, nBufSize, bQuietTruncation, ppszFormat, nPrevLen, lastArg ) \
	do \
	{ \
		int     result; \
		va_list arg_ptr; \
		bool bTruncated = false; \
		static int scAsserted = 0; \
	\
		va_start(arg_ptr, lastArg); \
		result = V_vsnprintfRet( (szBuf), (nBufSize)-1, (*(ppszFormat)), arg_ptr, &bTruncated ); \
		va_end(arg_ptr); \
	\
		(szBuf)[(nBufSize)-1] = 0; \
		if ( bTruncated && !(bQuietTruncation) && scAsserted < 5 ) \
		{ \
			Warning( "FmtStrVSNPrintf truncated to %d without QUIET_TRUNCATION specified!\n", ( int )( nBufSize ) ); \
			AssertMsg( 0, "FmtStrVSNPrintf truncated without QUIET_TRUNCATION specified!\n" ); \
			scAsserted++; \
		} \
		m_nLength = nPrevLen + result; \
	} \
	while (0)

// using macro to be compatable with GCC
#define FmtStrVSNPrintfNoLengthFixup( szBuf, nBufSize, bQuietTruncation, ppszFormat, nPrevLen, lastArg ) \
	do \
	{ \
		int     result; \
		va_list arg_ptr; \
		bool bTruncated = false; \
		static int scAsserted = 0; \
	\
		va_start(arg_ptr, lastArg); \
		result = V_vsnprintfRet( (szBuf), (nBufSize)-1, (*(ppszFormat)), arg_ptr, &bTruncated ); \
		va_end(arg_ptr); \
	\
		(szBuf)[(nBufSize)-1] = 0; \
		if ( bTruncated && !(bQuietTruncation) && scAsserted < 5 ) \
		{ \
			Warning( "FmtStrVSNPrintf truncated to %d without QUIET_TRUNCATION specified!\n", ( int )( nBufSize ) ); \
			AssertMsg( 0, "FmtStrVSNPrintf truncated without QUIET_TRUNCATION specified!\n" ); \
			scAsserted++; \
		} \
	} \
	while (0)

//-----------------------------------------------------------------------------
//
// Purpose: String formatter with specified size
//

template <int SIZE_BUF, bool QUIET_TRUNCATION = false >
class CFmtStrN
{
public:
	CFmtStrN()	
	{ 
		InitQuietTruncation();
		m_szBuf[0] = 0; 
		m_nLength = 0;
	}
	
	// Standard C formatting
	CFmtStrN(const char *pszFormat, ...)
	{
		InitQuietTruncation();
		FmtStrVSNPrintf( m_szBuf, SIZE_BUF, m_bQuietTruncation, &pszFormat, 0, pszFormat );
	}

	// Use this for pass-through formatting
	CFmtStrN(const char ** ppszFormat, ...)
	{
		InitQuietTruncation();
		FmtStrVSNPrintf( m_szBuf, SIZE_BUF, m_bQuietTruncation, ppszFormat, 0, ppszFormat );
	}

	// Explicit reformat
	const char *sprintf(const char *pszFormat, ...)
	{
		InitQuietTruncation();
		FmtStrVSNPrintf(m_szBuf, SIZE_BUF, m_bQuietTruncation, &pszFormat, 0, pszFormat ); 
		return m_szBuf;
	}

	// Use this for va_list formatting
	const char *sprintf_argv(const char *pszFormat, va_list arg_ptr)
	{
		int result; 
		bool bTruncated = false; 
		static int s_nWarned = 0; 

		InitQuietTruncation();
		result = V_vsnprintfRet( m_szBuf, SIZE_BUF - 1, pszFormat, arg_ptr, &bTruncated );
		m_szBuf[SIZE_BUF - 1] = 0; 
		if ( bTruncated && !m_bQuietTruncation && ( s_nWarned < 5 ) ) 
		{ 
			Warning( "CFmtStr truncated to %d without QUIET_TRUNCATION specified!\n", SIZE_BUF ); 
			AssertMsg( 0, "CFmtStr truncated without QUIET_TRUNCATION specified!\n" );
			s_nWarned++; 
		} 
		m_nLength = V_strlen( m_szBuf );
		return m_szBuf;
	}

	// Use this for pass-through formatting
	void VSprintf(const char **ppszFormat, ...)
	{
		InitQuietTruncation();
		FmtStrVSNPrintf( m_szBuf, SIZE_BUF, m_bQuietTruncation, ppszFormat, 0, ppszFormat );
	}

	// Compatible API with CUtlString for converting to const char*
	const char *Get( ) const					{ return m_szBuf; }
	const char *String( ) const					{ return m_szBuf; }
	// Use for access
	operator const char *() const				{ return m_szBuf; }
	char *Access()								{ return m_szBuf; }

	// Access template argument
	static inline int GetMaxLength() { return SIZE_BUF-1; }

	CFmtStrN<SIZE_BUF,QUIET_TRUNCATION> & operator=( const char *pchValue ) 
	{ 
		V_strncpy( m_szBuf, pchValue, SIZE_BUF );
		m_nLength = V_strlen( m_szBuf );
		return *this; 
	}

	CFmtStrN<SIZE_BUF,QUIET_TRUNCATION> & operator+=( const char *pchValue ) 
	{ 
		Append( pchValue ); 
		return *this; 
	}

	int Length() const							{ return m_nLength; }

	void SetLength( int nLength )
	{
		m_nLength = Min( nLength, SIZE_BUF - 1 );
		m_szBuf[m_nLength] = '\0';
	}

	void Clear()								
	{ 
		m_szBuf[0] = 0; 
		m_nLength = 0; 
	}

	void AppendFormat( const char *pchFormat, ... )
	{ 
		char *pchEnd = m_szBuf + m_nLength; 
		FmtStrVSNPrintf( pchEnd, SIZE_BUF - m_nLength, m_bQuietTruncation, &pchFormat, m_nLength, pchFormat ); 
	}

	void AppendFormatV( const char *pchFormat, va_list args );
	
	void Append( const char *pchValue )
	{
		// This function is close to the metal to cut down on the CPU cost
		// of the previous incantation of Append which was implemented as
		// AppendFormat( "%s", pchValue ). This implementation, though not
		// as easy to read, instead does a strcpy from the existing end
		// point of the CFmtStrN. This brings something like a 10-20x speedup
		// in my rudimentary tests. It isn't using V_strncpy because that
		// function doesn't return the number of characters copied, which
		// we need to adjust m_nLength. Doing the V_strncpy with a V_strlen
		// afterwards took twice as long as this implementations in tests,
		// so V_strncpy's implementation was used to write this method.
		char *pDest = m_szBuf + m_nLength;
		const int maxLen = SIZE_BUF - m_nLength;
		char *pLast = pDest + maxLen - 1;
		while ( (pDest < pLast) && (*pchValue != 0) )
		{
			*pDest = *pchValue;
			++pDest; ++pchValue;
		}
		*pDest = 0;
		m_nLength = pDest - m_szBuf;
	}

	//optimized version of append for just adding a single character
	void Append( char ch )
	{
		if( m_nLength < SIZE_BUF - 1 )
		{
			m_szBuf[ m_nLength ] = ch;
			m_nLength++;
			m_szBuf[ m_nLength ] = '\0';
		}
	}

	void AppendIndent( uint32 unCount, char chIndent = '\t' );

	void SetQuietTruncation( bool bQuiet ) { m_bQuietTruncation = bQuiet; }

protected:
#if defined(TF_CLASSIC) || defined(TF_CLASSIC_CLIENT)
	void InitQuietTruncation()
#else
	virtual void InitQuietTruncation()
#endif
	{
		m_bQuietTruncation = QUIET_TRUNCATION; 
	}

	bool m_bQuietTruncation;

private:
	char m_szBuf[SIZE_BUF];
	int m_nLength;
};


// Version which will not assert if strings are truncated

template < int SIZE_BUF >
class CFmtStrQuietTruncationN : public CFmtStrN<SIZE_BUF, true >
{
};


template< int SIZE_BUF, bool QUIET_TRUNCATION >
void CFmtStrN< SIZE_BUF, QUIET_TRUNCATION >::AppendIndent( uint32 unCount, char chIndent )
{
	Assert( Length() + unCount < SIZE_BUF );
	if( Length() + unCount >= SIZE_BUF )
		unCount = SIZE_BUF - (1+Length());
	for ( uint32 x = 0; x < unCount; x++ )
	{
		m_szBuf[ m_nLength++ ] = chIndent;
	}
	m_szBuf[ m_nLength ] = '\0';
}

template< int SIZE_BUF, bool QUIET_TRUNCATION >
void CFmtStrN< SIZE_BUF, QUIET_TRUNCATION >::AppendFormatV( const char *pchFormat, va_list args )
{
	int cubPrinted = V_vsnprintf( m_szBuf+Length(), SIZE_BUF - Length(), pchFormat, args );
	m_nLength += cubPrinted;
}


#if defined(POSIX)
#pragma GCC visibility pop
#endif

//-----------------------------------------------------------------------------
//
// Purpose: Default-sized string formatter
//

#define FMTSTR_STD_LEN 256

typedef CFmtStrN<FMTSTR_STD_LEN> CFmtStr;
typedef CFmtStrQuietTruncationN<FMTSTR_STD_LEN> CFmtStrQuietTruncation;
typedef CFmtStrN<1024> CFmtStr1024;
typedef CFmtStrN<8192> CFmtStrMax;

#endif // FMTSTR_H
