// xml.h
// v.118
// revision 31 - 5 - 2007
#ifndef _XML_H
#define _XML_H

#include "GeneralIncludes.h"

#ifdef __BORLANDC__
#pragma warn -pck
#endif

#ifdef _MSC_VER
#define _USERENTRY __cdecl
#pragma warning(disable : 4996)
#pragma warning(disable : 4068)
#endif

#ifdef LINUX
#define _USERENTRY
#endif


#define XML_VERSION 0x126
#define XML_VERSION_REVISION_DATE "31-05-2007"

// ANSI includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#ifdef _WIN32
#ifndef __SYMBIAN32__
#include <windows.h>
#include <wininet.h>
#include <tchar.h>
#endif
#endif

#ifdef __SYMBIAN32__
#define _USERENTRY
#define strcmpi strcmp
#include <unistd.h>
#endif


#ifndef __SYMBIAN32__
#ifdef XML_USE_STL
#include <vector>
using namespace std;
#endif
#endif

#ifndef _Z_H
#define _Z_H
// Z template class
template <class T>class Z
	{
   private:

   	T* d;
      unsigned int ss;

   public:

   	Z(int s = 0)
      {
			if (s == 0)
				s = 1;
		   d = new T[s];
		   memset(d,0,s*sizeof(T));
         ss = s;
      }
      ~Z()
      {
	      delete[] d;
      }

      operator T*()
      {
         return d;
      }

   void _clear()
		{
      ZeroMemory(d,ss*sizeof(T));
      }


   };
#endif // Z_H



class XMLHeader;
class XMLElement;
class XMLVariable;
class XMLComment;
class XMLContent;
class XML;

typedef struct
	{
   int VersionHigh;
   int VersionLow;
   char RDate[20];
   } XML_VERSION_INFO;


#ifdef _WIN32
struct IMPORTDBTABLEDATA
   {
   char name[256];
   char itemname[100];
   int nVariables;
   char** Variables;
   char** ReplaceVariables;
   };

struct IMPORTDBPARAMS
   {
   char* dbname;
   char* provstr;
   int nTables;
   IMPORTDBTABLEDATA* Tables;
   };
#endif

struct XMLEXPORTFORMAT
	{
	bool UseSpace;
	int nId;
	};

#ifdef _WIN32
struct IMPORTRKEYDATA
   {
   HKEY pK;
   int StorageType; // 0 - Native
					// 1 - Registry key from native XML
					// 2 - Registry key from registry XML
   };
#endif


// PARTIAL LOAD mode
struct XMLPARTIALLOADOPTIONS
	{
	int ElementDeep; // 0 = root, or -1 if to ignore it
	char* ElementName; // If valid, we only return element by that name
	};


// Global functions

class XML
   {
   public:

      // constructors/destructor

      XML( void );
      XML( const char* file,int LoadMode = 0,class XMLTransform* eclass = 0,class XMLTransformData* edata = 0);
      void Version(XML_VERSION_INFO*);
      int MemoryUsage();
		void CompressMemory();
		bool IntegrityTest();
		int Compare(XML*);

	  XML(XML& x);
	  XML& operator =(XML&);
	  ~XML();

//      static void Kill(char* tf);
	  static int DoMatch(char *text, char *p, bool IsCaseSensitive = false);
	  static bool VMatching(char *text, char *p, bool IsCaseSensitive = false);
	  static bool TestMatch(const char* item1,const char* comp,const char* item2);
	  static Z<char>* ReadToZ(const char*,class XMLTransform* eclass = 0,class XMLTransformData* edata = 0);
	  static int Convert2HexCharsToNumber(int c1, int c2);
	  static XMLElement* Paste();

	  int ParseStatus(int* = 0);
	  void SaveOnClose(bool);
	  int Load(const char* data,int LoadMode = 0,class XMLTransform* eclass = 0,class XMLTransformData* edata = 0);
	  int PartialLoad(const char* file,XMLPARTIALLOADOPTIONS* d);
	  static XMLElement * PartialElement(const char* file,XMLPARTIALLOADOPTIONS* d);
	  int Save(const char* file = 0,int SaveMode = 1,int TargetMode = 0,class XMLTransform* eclass = 0,class XMLTransformData* edata = 0); // Default, do not encode already encoded
      void Export(FILE* fp,int SaveMode,int TargetMode = 0,XMLHeader *hdr = 0,class XMLTransform* eclass = 0,class XMLTransformData* edata = 0);
	  void SetExportFormatting(XMLEXPORTFORMAT* xf);

      void Lock(bool);
      void SetRootElement(XMLElement*);
      XMLElement* GetRootElement();
      XMLHeader* GetHeader();
			void SetHeader(XMLHeader* h);
#ifdef XML_OPTIONAL_IMPORTDB
      static XMLElement* ImportDB(IMPORTDBPARAMS* );
#endif
#ifdef XML_OPTIONAL_IMPORTRKEY
      static XMLElement* ImportRKey(IMPORTRKEYDATA*);
#endif
      static int XMLEncode(const char* src,char* trg);
      static int XMLDecode(const char* src,char* trg);
      int XMLGetValue(const char* section,char* attr,char* put,int maxlen);
      void XMLSetValue(const char* section,char* attr,char* put);

      // Query functions
      int XMLQuery(const char* rootsection,const char* expression,XMLElement** rv,unsigned int deep = 0xFFFFFFFF);

   private:

      void Init();
      void Clear();

      int iParseStatus; // 0 Valid , 1 Error but recovered, 2 fatal error
      int iParseStatusPos;
      char* f;          // filename
      XMLHeader* hdr;   // header (one)
      XMLElement* root; // root element (one)
      bool SOnClose;
      // For Windows

   };

#ifdef LINUX
		typedef int (*fcmp) (const void *, const void *);
#endif

class XMLElement
   {
   public:

      // constructors/destructor
      XMLElement(XMLElement* par,const char* el,int Type = 0);

	  //XMLElement& operator =(XMLElement&);
	  ~XMLElement();

		XMLElement* operator[](int);
		void SetElementParam(unsigned long p);
		unsigned long GetElementParam();
      void Reparse(const char*el,int Type = 0);
      int GetDeep();
      int AddElement(XMLElement*);
      int InsertElement(unsigned int,XMLElement*);
      int FindElement(XMLElement*);
      int FindElement(const char* n);
      XMLElement* FindElementZ(XMLElement*);
      XMLElement* FindElementZ(const char* n,bool ForceCreate = false,char* el = 0);
	  int RemoveElement(unsigned int i);
	  int RemoveElementAndKeep(unsigned int i,XMLElement** el);
	  int RemoveAllElements();
	  int MoveElement(unsigned int i,unsigned int y);
#ifdef LINUX
      void SortElements(fcmp);
      void SortVariables(fcmp);
      friend int XMLElementfcmp(const void *, const void *);
      friend int XMLVariablefcmp(const void *, const void *);
#else
      void SortElements(int (_USERENTRY *fcmp)(const void *, const void *));
      void SortVariables(int (_USERENTRY *fcmp)(const void *, const void *));
      friend int _USERENTRY XMLElementfcmp(const void *, const void *);
      friend int _USERENTRY XMLVariablefcmp(const void *, const void *);
#endif
      XMLElement* Duplicate(XMLElement* = 0);
		void Copy();
		int MemoryUsage();
		void CompressMemory();
		bool IntegrityTest();
		int Compare(XMLElement*);

	  // XMLComment
	  XMLComment** GetComments();
	  unsigned int GetCommentsNum();
	  int AddComment(XMLComment*,int InsertBeforeElement);
	  int RemoveComment(unsigned int i);
      int RemoveAllComments();


      int AddVariable(XMLVariable*);
      int FindVariable(XMLVariable*);
      int FindVariable(char*  x);
      XMLVariable* FindVariableZ(XMLVariable*);
      XMLVariable* FindVariableZ(char* x,bool ForceCreate = false,char* defnew = 0);
	  int RemoveVariable(unsigned int i);
	  int RemoveVariableAndKeep(unsigned int i,XMLVariable** vr);
	  int RemoveAllVariables();

	  int AddContent(XMLContent* v,int InsertBeforeElement);
	  int RemoveContent(unsigned int i);
	  void RemoveAllContents();
	  XMLContent** GetContents();
		unsigned int GetContentsNum();

      XMLElement* GetElementInSection(const char*);
      int XMLQuery(const char* expression,XMLElement** rv,unsigned int deep = 0xFFFFFFFF);

      XMLElement* GetParent();

      XMLElement** GetChildren();
		unsigned int GetChildrenNum();
      unsigned int GetAllChildren(XMLElement**,unsigned int deep = 0xFFFFFFFF);
      unsigned int GetAllChildrenNum(unsigned int deep = 0xFFFFFFFF);

      XMLVariable** GetVariables();
      unsigned int GetVariableNum();
      void Export(FILE* fp,int ShowAll,int SaveMode,int TargetMode = 0,XMLHeader* hdr = 0,class XMLTransform* eclass = 0,class XMLTransformData* edata = 0);
	  void SetExportFormatting(XMLEXPORTFORMAT* xf);

      void SetElementName(const char*);
      int GetElementName(char*,bool NoDecode = false);
      int GetElementFullName(char*,bool NoDecode = false);

      int   GetType();

   private:

		  XMLElement(XMLElement&);
      unsigned long param;
      char* el; // element name
      int type; // type, 0 element
      XMLElement* parent; // one
      XMLElement** children; // many
      XMLVariable** variables; // many
	  XMLComment** comments; // many
	  XMLContent** contents; // many;
      unsigned int childrennum;
      unsigned int variablesnum;
	  unsigned int commentsnum;
	  unsigned int contentsnum;


      int SpaceForElement(unsigned int);
      int SpaceForVariable(unsigned int);
      int SpaceForComment(unsigned int);
      int SpaceForContent(unsigned int);

      int TotalChildPointersAvailable;
      int TotalVariablePointersAvailable;
      int TotalCommentPointersAvailable;
      int TotalContentPointersAvailable;
		XMLEXPORTFORMAT xfformat;

      static void printc(FILE* fp,XMLElement* root,int deep,int ShowAll,int SaveMode,int TargetMode);
      void SetParent(XMLElement*);


   };


class XMLVariable
	{
	public:

		XMLVariable(const char*,const char*,bool NoDecode = false);
		~XMLVariable();
	  XMLVariable(XMLVariable&);
	  XMLVariable& operator =(XMLVariable&);


		int MemoryUsage();
		void CompressMemory();
		bool IntegrityTest();
		int Compare(XMLVariable*);
		XMLElement* SetOwnerElement(XMLElement*);
		int GetName(char*,bool NoDecode = false);
		int GetValue(char*,bool NoDecode = false);
		int GetValueInt();
		void SetName(const char*,bool NoDecode = false);
		void SetValue(const char*,bool NoDecode = false);
		void SetValueInt(int);
		XMLVariable* Duplicate();
		void Copy();

#ifdef XML_OPTIONAL_MIME
		int GetBinaryValue(char*);
		int SetBinaryValue(char*,int);
#endif

	private:

		void Clear();
		char* vn;
		char* vv;
	  XMLElement* owner;


	};


class XMLHeader
	{
	public:

		// constructors/destructor
      XMLHeader(const char* ht);
      operator char*();
      int MemoryUsage();
		void CompressMemory();
		bool IntegrityTest();
		int Compare(XMLHeader*);
		XMLHeader* Duplicate();


	  XMLHeader(XMLHeader&);
	  XMLHeader& operator =(XMLHeader&);
      ~XMLHeader();

      // XMLComment
      XMLComment** GetComments();
		unsigned int GetCommentsNum();
      int AddComment(XMLComment*,int pos);
      int RemoveComment(unsigned int i);
      int RemoveAllComments();
      int SpaceForComment(unsigned int);

      void Export(FILE* fp,int SaveMode,int TargetMode = 0,class XMLTransform* eclass = 0,class XMLTransformData* edata = 0);

   private:

   	  void Clear();
      int TotalCommentPointersAvailable;
      char* hdr;
      unsigned int commentsnum;
		XMLComment** comments;
   };


class XMLComment
   {
   public:

	  // constructors/destructor
	  XMLComment(XMLElement* p,int ElementPosition,const char* ht);
	  operator char*();
	  void SetComment(const char* ht);
	  int MemoryUsage();
	  void CompressMemory();
	  bool IntegrityTest();
	  int Compare(XMLComment*);

	  XMLComment(XMLComment&);
	  XMLComment& operator =(XMLComment&);
	  ~XMLComment();

	  XMLComment* Duplicate();
	  void SetParent(XMLElement* p,int ep);
	  int GetEP();

	private:

	  XMLElement* parent;
	  char* c;
	  int ep; // Element Position (Before)
	};


class XMLContent
   {
   public:

	  // constructors/destructor
	  XMLContent(XMLElement* p,int ElementPosition,const char* ht,bool NoDecode = false);
	  int GetValue(char*,bool NoDecode = false);
	  void SetValue(const char*,bool NoDecode = false);
	  int MemoryUsage();
	  void CompressMemory();
	  bool IntegrityTest();
	  int Compare(XMLContent*);

		XMLContent(XMLContent&);
	  XMLContent& operator =(XMLContent&);
	  ~XMLContent();

	  XMLContent* Duplicate();
	  void SetParent(XMLElement* p,int ep);
	  int GetEP();

   private:

	  XMLElement* parent;
	  char* c;
	  int ep; // Element Position (Before)
   };


// public functions
int XMLGetString(const char* section,const char* Tattr,const char* defv,char*out,const int maxlen,const char* xml,XML* af = 0);
int     XMLGetInt(const char* item,const char* attr,const int defv,const char* xml,XML* af = 0);
#ifdef _WIN32
__int64 XMLGetInt64(const char* item,const char* attr,const __int64 defv,const char* xml,XML* af = 0);
#endif
float   XMLGetFloat(const char* item,const char* attr,const float defv,const char* xml,XML* af = 0);
int     XMLGetBinaryData(const char* item,const char* attr,const char* defv,char*out,const int maxlen,const char* xml,XML* af = 0);

int    XMLSetString(const char* section,const char* Tattr,char*out,const char* xml,XML* af = 0);
int    XMLSetInt(char* section,char* attr,int v,char* xml,XML* af = 0);
#ifdef _WIN32
int    XMLSetString(const char* section,const char* Tattr,wchar_t*out,const char* xml,XML* af = 0);
int    XMLSetInt64(char* section,char* attr,__int64 v,char* xml,XML* af = 0);
#endif
int    XMLSetFloat(char* section,char* attr,float v,char* xml,XML* af = 0);
int    XMLSetBinaryData(char* section,char* attr,char* data,int len,char* xml,XML* af = 0);

int XMLRenameElement(const char* section,const char* newname,const char* xml,XML* af = 0);

#ifndef __SYMBIAN32__
#ifdef XML_USE_STL
int    XMLGetAllVariables(char* section,vector<char*>* vnames,vector<char*>* vvalues,char*xml);
int    XMLGetAllItems(char* section,vector<char*>* vnames,char*xml);
#else
int    XMLGetAllVariables(char* section,char** vnames,char** vvalues,char*xml);
int    XMLGetAllItems(char* section,char** vnames,char*xml);
#endif
#endif



// XMLTransform class

class XMLTransformData
   {
   public:
      XMLTransformData() {}
   };

class XMLTransform
   {
   public:

      XMLTransform(XMLTransformData*) { }
      virtual int Encrypt(const char*src,int srclen,int srctype,char* dst,int dstlen,XMLTransformData* data = 0) = 0;
      virtual int Decrypt(const char*src,int srclen,int srctype,char* dst,int dstlen,XMLTransformData* data = 0) = 0;

   };





#endif // _XML_H


