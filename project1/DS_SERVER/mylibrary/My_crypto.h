/*================================================================================
 by ozzywow
=================================================================================*/




#pragma once


// compile option


struct _sHeader {
	short		sLength : 12 ;		// -2048 ~ 2047
	short		sCrypto : 2 ;		// 0 : false , �������� true
	short		sCompressed : 2 ;	//0 : false , �������� true
};


#ifndef IN
#define IN
#define OUT
#endif


#define KEYVALLENTH			16			// ��������Ű�� �ִ� ũ��
#define KEY_RANGE			0xff		// Ű ����..

/*================================================================================
 CLASS NAME : _j_Crypto 
 EXP		: ����Ÿ ��ȣȭ / ��ȣȭ
 AUTHOR		: ozzywow
 DATE		: 2003.11. ,   SE : 2005,4,07
=================================================================================*/


class _j_Crypto
{

private:	
	
	DWORD	m_dwKeyLength;									// Key �� ����	
	char m_cKeyBox[KEY_RANGE][KEYVALLENTH];					// Key ���� ����ִ� BOX	
	
	int		m_iBufMaxSize ;
	int		m_iBufCurSet ;
	

public:		
	_j_Crypto::_j_Crypto( size_t bufSize ) : m_iBufMaxSize( bufSize ), m_iBufCurSet(0)
	{
		memset(m_cKeyBox,0,sizeof(m_cKeyBox));		
	}
	_j_Crypto::~_j_Crypto()
	{
	}

	
	// �ʱ�ȭ (Ű�� ����)
	// pathName : Ű ������ ����� ���� �̸�..
	// Ŭ������ �����ұ� �ʱ� ��ǥ�� (x,y) �� �־ �ʱ�ȭ�� �ּ���.
	bool _j_Crypto::Init(const char * pathName)
	{
		if( pathName )
		{
			//Ű ���� �о����..
			FILE *	stream = NULL;
			try{
				stream = fopen(pathName,"rb");
			}
			catch (...) {		
				fclose(stream);
				return false;
			}
			
			if(stream == NULL) {
				//fclose(stream);
				return false;
			}		
			
			//while(!feof(stream)) {
			//	if(fread(&m_cKeyBox[0][0][0], sizeof(char), sizeof(m_cKeyBox), stream) == NULL) ;//break;
			//}
			memset( m_cKeyBox, 0, sizeof(m_cKeyBox) ) ;

			fread(m_cKeyBox, sizeof(m_cKeyBox) , 1, stream) ;
			
			if(fclose(stream) != 0) return false;
			
			// Ű ���� �б� ��
		}
		return true ;
	}
	
	//��ȣȭ//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// �� �Լ��� ����Ÿ�� ��ȣȭ �Ѵ�.   									//
	// �Լ� : Encryption(char * pData, USHORT usDataLen)					//
	// ���� : Ű�� ��Ʈ ������ �ϰ� �밢���� ��ġ �������� ����Ÿ�� ȥ��������//
	//////////////////////////////////////////////////////////////////////////
	inline bool _j_Crypto::Encryption( IN OUT char * pData, const USHORT usDataLen, const u_char key )
	{
	
		// Get key string
		char * cValKey ;
		UINT * uiValKey ;
		cValKey = GetKey( key ) ;		
		uiValKey = (UINT*)cValKey ;	
		

		try {

			int x, i,j,k,l ;
			
			UINT * puiData = (UINT *) pData ;						// 'int *' type data �� ��ȯ		

			j = 0 ;							// line step
			k = usDataLen / KEYVALLENTH ;	// line count
			l = usDataLen % KEYVALLENTH ;	// char count (������ ����Ÿ)			
		
			
			// line data block Encrypto
			for( i = 0, x = 0, j = 0 ; i < k ; i++, x += 4, j += 16 ) {	// Key �� pData �� ��ġ�Ѵ�.				
				puiData[x+0] = uiValKey[0] ^ puiData[x+0] ;
				puiData[x+1] = ~(uiValKey[2] ^ puiData[x+1]) ;
				puiData[x+2] = uiValKey[1] ^ puiData[x+2] ;
				puiData[x+3] = ~(uiValKey[3] ^ puiData[x+3]) ;
			}
			
			// char data Encrypto (������ ����Ÿ)
			//j += KEYVALLENTH ;
			for(int h = 0 ; h < l ; h++ ) {
				pData[j+h] = ~(cValKey[h] ^ pData[j+h]) ;
			}
			
		} // end try
		catch (...) {
			return false ;
		}	
		return true ;
		
	}
	//��ȣȭ//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// �� �Լ��� ����Ÿ�� ��ȣȭ �Ѵ�.   									//
	// �Լ� : Decryption(char * pData, USHORT usDataLen)						//
	// ���� : Ű�� ��Ʈ ������ �ϰ� �밢���� ��ġ �������� ����Ÿ�� ȥ��������//
	//////////////////////////////////////////////////////////////////////////
	inline bool _j_Crypto::Decryption(char * pData, const USHORT usDataLen, const u_char key )
	{	

		// Get key string
		char * cValKey ;
		UINT * uiValKey ;
		cValKey = GetKey( key ) ;		
		uiValKey = (UINT*)cValKey ;	

		try {	
			
			int x, i,j,k,l ;
			
			UINT * puiData = (UINT *) pData ;						// 'int *' type data �� ��ȯ		
			
			j = 0 ;							// line step
			k = usDataLen / KEYVALLENTH ;	// line count
			l = usDataLen % KEYVALLENTH ;	// char count (������ ����Ÿ)			
			
			
			// line data block Encrypto
			for( i = 0, x = 0, j = 0 ; i < k ; i++, x += 4, j += 16 ) {	// Key �� pData �� ��ġ�Ѵ�.				
				puiData[x+0] = uiValKey[0] ^ puiData[x+0] ;
				puiData[x+1] = ~(uiValKey[2] ^ puiData[x+1]) ;
				puiData[x+2] = uiValKey[1] ^ puiData[x+2] ;
				puiData[x+3] = ~(uiValKey[3] ^ puiData[x+3]) ;
			}
			
			// char data Encrypto (������ ����Ÿ)
			//j += KEYVALLENTH ;
			for(int h = 0 ; h < l ; h++ ) {
				pData[j+h] = ~(cValKey[h] ^ pData[j+h]) ;
			}

		}// try
		catch (...) {
			return false ;
		}
		return true ;		
	}

	//////////////////////////////////////////////////////////////////////////
	// �� �Լ��� ����Ÿ�� ��/��ȣȭ �Ѵ�.									//
	// �Լ� : Xor(char * pData, USHORT usDataLen)	�μ� : ���ڿ� ������(plantext)	//
	// ���� : ���ڿ��� �Էµ� Ű�� �ܼ��� XOR ������ �����Ѵ�.              //
	//////////////////////////////////////////////////////////////////////////
	inline char * _j_Crypto::Xor(char *pData, USHORT usDataLen, const u_char key )
	{	
		char * cValKey ;
		cValKey = GetKey( key ) ;		
		
		for(USHORT i = 0; i < usDataLen; i++){
			pData[i]=~ pData[i]^cValKey[i % KEYVALLENTH];
		}

		return pData;		
	}	

	
	inline u_char _j_Crypto::checksum(u_char *buffer, int size) {
		
		unsigned long cksum=0;
		
		while(size >1) {
			cksum+=*buffer++;
			size -=sizeof(u_char);
		}
		
		if(size ) {
			cksum += *(u_char*)buffer;
		}
		
		cksum = (cksum >> 16) + (cksum & 0xffff);
		cksum += (cksum >>16);
		return (u_char)(~cksum);
	}


private:
	inline char * _j_Crypto::GetKey(const u_char key , const char * pathName = NULL )	
	{		
		return m_cKeyBox[key % KEY_RANGE];
	}	
};



struct _Tail {
	u_char		crc ;
	u_char		seq ;
};

struct _Encapsule_info {	
	char *		buf ;
	u_short		length ;
};

struct _Decapsule_info {	
	char *		buf ;
	u_short		length ;
	u_char		seq ;
};


#define _CAPSULE_BUF_SIZE_		4096
#define _TAIL_SIZE_				2

class CCapsulateCrypto : public _j_Crypto
{

private:
	char *	m_pCapsule_buf ;
	int		m_iBufMaxSize ;
	int		m_iBufCurSet ;
	u_char	m_ucSeq ;

public:

	CCapsulateCrypto(size_t bufSize = _CAPSULE_BUF_SIZE_ ) : _j_Crypto(bufSize), m_iBufMaxSize(bufSize), m_iBufCurSet(0), m_ucSeq(0)
	{
		m_pCapsule_buf = new char[bufSize] ;
	}



	~CCapsulateCrypto()
	{		
		delete [] m_pCapsule_buf ;
	}

	inline void InitSeqNum( u_char seq ) { m_ucSeq = seq ; }
	
	inline _Encapsule_info Encapsulate( char * packet, u_char key = 0  ) 
	{
		if( key == 0 ) 	{ key = m_ucSeq++ ; }
		_sHeader * pHeader = (_sHeader*)packet ;
		_Encapsule_info capsuleInfo ;
		
		if( pHeader->sLength + _TAIL_SIZE_ > m_iBufMaxSize )
		{
			capsuleInfo.buf = NULL ;
			capsuleInfo.length = 0 ;
			return capsuleInfo ;
		}
		
		memcpy( m_pCapsule_buf, packet, pHeader->sLength ) ;
		packet = m_pCapsule_buf ;		
	
		//  Add tail
		pHeader = (_sHeader*)packet ;
		_Tail * pTail = (_Tail*)&packet[pHeader->sLength] ;
		pTail->crc = checksum( (u_char*)packet, pHeader->sLength ) ;
		pTail->seq = key ;
		pHeader->sLength +=  _TAIL_SIZE_ ;		
		
		// encryption
		if( pHeader->sCrypto == 1 )
		{
			if( Encryption( (char*)&packet[2], pHeader->sLength, key ) == false )			
			{
				capsuleInfo.buf = NULL ;
				capsuleInfo.length = 0 ;
				return capsuleInfo ;
			}
		}
		
		capsuleInfo.buf = packet ;
		capsuleInfo.length = pHeader->sLength ;
		
		return capsuleInfo ;
	}

	inline _Decapsule_info Decapsulate( char * packet, u_char key = 0  ) 
	{
		if( key == 0 ) 	{ key = m_ucSeq ; }
		_sHeader * pHeader = (_sHeader*)packet ;
		_Decapsule_info	decapsule_info ={0};
		if( pHeader->sCrypto == 1 )
		{
			if( Decryption( (char*)&packet[2], pHeader->sLength, key ) == false )
			{
				decapsule_info.buf = NULL ;
				decapsule_info.length = 0 ;
				return decapsule_info ;
			}
		}

		pHeader->sLength -= _TAIL_SIZE_ ;
		_Tail * pTail = (_Tail*)&packet[pHeader->sLength] ;
		if( pTail->crc == checksum( (u_char*)packet, pHeader->sLength ) )
		{
			decapsule_info.buf = packet ;
			decapsule_info.length = pHeader->sLength ;			
		}
		else
		{
			decapsule_info.buf = NULL ;
			decapsule_info.length = 0 ;
		}
		decapsule_info.seq = pTail->seq ;

		return decapsule_info ;
	}
};

