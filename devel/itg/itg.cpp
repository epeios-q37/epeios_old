/*
  'itg' library by Claude L. Simon (simon@epeios.org)
  Requires the 'itg' header file ('itg.h').
  Copyright (C) 2000,2001 Claude L. SIMON (simon@epeios.org).

  This file is part of the Epeios (http://www.epeios.org/) project.
  

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
 
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, go to http://www.fsf.org/
  or write to the:
  
                        Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

//	$Id$

#define ITG__COMPILATION

#include "itg.h"

class itgtutor
: public ttr_tutor
{
public:
	itgtutor( void )
	: ttr_tutor( ITG_NAME )
	{
#ifdef ITG_DBG
		Version = ITG_VERSION "\b\bD $";
#else
		Version = ITG_VERSION;
#endif
		Owner = ITG_OWNER;
		Date = "$Date$";
	}
	virtual ~itgtutor( void ){}
};

/******************************************************************************/
				  /* do not modify anything above this limit */
				  /*			  unless specified			 */
				  /*******************************************/
/*$BEGIN$*/

namespace itg {
	integer_ divide_(
		const integer_ &Num,
		const integer_ &Den,
		integer_ &Res )
	{
		bso__ulong Inf = 0UL, Sup = 0x10000UL, Rep = 0x10000UL >> 1;
		integer Inter, Comp = 0;
		
		for (;;)
			if ( ( Inter = Num - Den * integer( Rep ) ) < Comp )
			{
				Sup = Rep;
				Rep = ( Sup + Inf ) >> 1;
			}
			else if ( Inter >= Den )
			{
				Inf = Rep;
				Rep = Inf + ( ( Sup - Inf ) >> 1 );
					// ( Sup + Inf ) << 1 ne marche pas (dépassement de capacité)
					// ni ( Sup << 1 ) + ( Inf << 1 ) (perte de précision)
			}
			else
			{
				Res = Res * integer( 0x10000L ) + integer( Rep );
				return Inter;
			}
	}
	
	int Comp_(
		const integer_ &Op1,
		const integer_ &Op2 )
	{
		int Res;

		if ( Op1.GetSign_() < Op2.GetSign_() )
			return 1;
		else if ( Op2.GetSign_() < Op1.GetSign_() )
			return -1;

		if ( Op1.GetSize_() < Op2.GetSize_() )
			Res = -1;
		else if ( Op1.GetSize_() > Op2.GetSize_() )
			Res = 1;
		else
		{
			size__ Indice = Op2.GetSize_();

			do
			{
				Indice--;
			}
			while( ( Op1.Core( Indice )
					 == Op2.Core( (bso__ulong)Indice ) )
					&& Indice );

			if ( Op1.Core( Indice ) != Op2.Core( Indice ) )
				if ( Op1.Core( Indice ) > Op2.Core( Indice ) )
					Res = 1;
				else
					Res = -1;
			else return 0;

		}

		return (int)( Op1.GetSign_() ? -Res : Res );
	}		
	
	integer_ Add_(
		const integer_ &Op1,
		const integer_ &Op2 )
	{
		integer ResTemp;
		const integer_ *Max = &Op1, *Min = &Op2;
		bso__ushort Indice = 0;
		bso__ulong Retenue = 0;
		bso__ushort Limite;

		ResTemp.Init();

		if ( ( Max->GetSize_() ) < ( Min->GetSize_() ) )
		{
			Max = &Op2;
			Min = &Op1;
		}

		ResTemp.Core.Allocate( Max->GetSize_() + 1 );

		Limite = Min->GetSize_();

		while( Indice < Limite )
		{
			Retenue = ( Retenue >> 16 )
				  + Min->Core( Indice )
				  + Max->Core( Indice);

			ResTemp.Core.Write( Retenue & 0xffff, Indice++ );
		}

		Limite = Max->GetSize_();

		while( Indice < Limite )
		{
			Retenue = ( Retenue >> 16 )
				  + Max->Core( Indice );

			ResTemp.Core.Write( Retenue & 0xffff, Indice++ );
		}

		if ( Indice >= ITG_MAX_SIZE )
			ERRl();
			// ResTemp.reset();
		else
		{
			ResTemp.Core.Write( Retenue >> 16, Indice++ );
			ResTemp.PutSize_( Indice );
			ResTemp.PutSign_( 0 );
			ResTemp.Adjust_();
		}

		return ResTemp;
	}
		
	integer_ Sub_(
		const integer_ &Op1,
		const integer_ &Op2 )
	{
		integer Res;
		unsigned Indice = 0;
		long Limite;
		long Inter = 0;
		const integer_ *Max = &Op1, *Min = &Op2;

		Res.Init();

		if ( *Max < *Min )
		{
			Res.PutSign_( 1 );
			Max = &Op2;
			Min = &Op1;
		}
		else
			Res.PutSign_( 0 );

#ifdef CPE__GCC
		Res.Core.tym::E_MEMORY_( base__ )::Allocate( Max->GetSize_() );
#else
		Res.Core.Allocate( Max->GetSize_() );	// ICE with GCC V2.95.2
#endif
	
		Res.PutSize_( Max->GetSize_() );

		Limite = Min->GetSize_();

		while ( Indice < Limite )
		{
			if ( Max->Core( Indice ) < ( Inter += Min->Core( Indice ) ) )
			{
				Res.Core.Write(
					( ( Max->Core( Indice ) | ( 1 << 16 ) )
						- ( Inter & 0xffff ) ),
					Indice );

				Inter = 1;
			}
			else
			{
				Res.Core.Write( ( Max->Core(Indice) - ( Inter & 0xffff ) ), Indice );

				Inter = 0;
			}

			Indice++;
		}

		Limite = Max->GetSize_();

		while( Indice < Limite )
		{
			if ( Max->Core( Indice ) < Inter )

			{
				Res.Core.Write(
					( ( Max->Core( Indice ) | ( 1 << 16 ) )
						- ( Inter & 0xffff ) ),
						Indice );

				Inter = 1;
			}
			else
			{
				Res.Core.Write( ( Max->Core( Indice ) - ( Inter & 0xffff ) ), Indice );

				Inter = 0;
			}

			Indice++;
		}

		Res.Adjust_();

		return Res;
	}


	integer_ Mul_(
		const integer_ &Op1,
		const integer_ &Op2 )
	{
		integer Inter, Res;
		unsigned long Retenue;
		short LimiteS, LimiteP, IndiceS = 0, IndiceP = 0;
		const integer_ *Max = &Op1, *Min = &Op2;

		Inter.Init();
		Res.Init();

		Res = 0;

		if ( *Max < *Min )
		{
			Max = &Op2;
			Min = &Op1;
		}

		if ( ( Max->GetSize_() + Min->GetSize_() ) > ITG_MAX_SIZE )
			ERRl();

		Inter.PutSign_( 0 );

#ifdef CPE__GCC
		Inter.Core.tym::E_MEMORY_( base__)::Allocate( Max->GetSize_() + Min->GetSize_() );
#else
		Inter.Core.Allocate( Max->GetSize_() + Min->GetSize_() );	// ICE on GCC V2.95.2
#endif
		LimiteS = Min->GetSize_();
		LimiteP = Max->GetSize_();

		while ( IndiceS < LimiteS )
		{
			IndiceP = 0;
			Retenue = 0;

			while ( IndiceP < LimiteP )
			{
				Retenue = ( Retenue >> 16 )
					  + (unsigned long)Max->Core(IndiceP)
						* (unsigned long)Min->Core(IndiceS);
				Inter.Core.Write( Retenue & 0xffff, IndiceP+IndiceS );
				IndiceP++;
			}

			Inter.Core.Write( Retenue >> 16, IndiceP+IndiceS );
			Inter.PutSize_( IndiceP + IndiceS + 1 );

			Res += Inter;
			IndiceS++;

			for( unsigned long C = 0; C < LimiteS + LimiteP; C++ )
				Inter.Core.Write( 0, C );

		}

		Res.Adjust_();

		return Res;
	}
	

	integer_ Div_(
		const integer_ &Den,
		const integer_ &Num,
		integer_ &Reste )
	{
		integer Res;
		unsigned long I;

		Res.Init();
		Res = 0;

		if ( Den > Num )
		{
			Reste = Num;
			return Res;
		}

		Reste.Init();
		
#ifdef CPE__GCC
		Reste.Core.tym::E_MEMORY_( base__ )::Allocate( Num.GetSize_() );
#else
		Reste.Core.Allocate( Num.GetSize_() );	// ICE with GCC 2.95.2
#endif

		Reste.PutSize_( Den.GetSize_() );
		Reste.PutSign_( 0 );

		Reste.Core.Write( Num.Core, 0, ( sizeof( base__ ) * Den.GetSize_() ), ( I = Num.GetSize_() - Den.GetSize_() ) );

		while( I-- )
		{
			Reste = Divide_( Reste, Den, Res ) * integer( 0x10000UL ) + integer( Num.Core( I ) );
		}

		Reste = Divide_( Reste, Den, Res );

		return Res;
	}


		
	integer_ operator +(
		const integer_ &Op1,
		const integer_ &Op2 )
	{
		if ( Op1.GetSign_() && Op2.GetSign_() )
			return -Add_( -Op1, -Op2 );
		else if ( Op1.GetSign_() )
			return Sub_( Op2, -Op1 );
		else if ( Op2.GetSign_() )
			return Sub_( Op1, -Op2 );
		else
			return Add_( Op1, Op2 );
	}

	integer_ operator -(
		const integer_ &Op1,
		const integer_ &Op2 )
	{
		if ( Op1.GetSign_() && Op2.GetSign_() )
			return Sub_( -Op2, -Op1 );
		else if ( Op1.GetSign_() )
			return Add_( -Op1, Op2 );
		else if ( Op2.GetSign_() )
			return Add_( Op1, -Op2 );
		else
			return Sub_( Op1, Op2 );
	}
	
	integer_ operator *(
		const integer_ &Op1,
		const integer_ &Op2 )
	{
		if ( Op1.GetSign_() && Op2.GetSign_() )
			return Mul_( -Op2, -Op1 );
		else if ( Op1.GetSign_() )
			return -Mul_( -Op1, Op2 );
		else if ( Op2.GetSign_() )
			return -Mul_( Op1, -Op2 );
		else
			return Mul_( Op1, Op2 );
	}
	
	integer_ operator /(
		const integer_ &Num,
		const integer_ &Den )
	{
		integer Dummy;
		
		if ( Num.GetSign_() != Den.GetSign_() )
			return -Div_( Abs( Num ), Abs( Den ), Dummy );
		else
			return Div_( Abs( Num ), Abs( Den ), Dummy );
	}
	
	flw::oflow___ & operator <<(
		flw::oflow___ &Flow,
		const integer_ &Integer )
	{
		int Compteur, Chiffre;

		if ( !Intger )
			return Flow << 0;
		else if ( Integer < integer( 0 ) )
		{
			Flow << '-';
			Integer = -Integer;
		}

		Compteur = (int)log10( Entier.LongFloat() );

		while( Compteur )
		{
			Chiffre =  (int)( Entier.GetLongFloat() / pow( 10, Compteur ) );

			Flow << Chiffre;

			Intger -= Exp( integer( 10 ), Compteur ) * integer( Chiffre );
			Compteur--;
		}

		return Flow << Entier.GetUShort_();
	}

	
}

/* Although in theory this class is inaccessible to the different modules,
it is necessary to personalize it, or certain compiler would not work properly */

class itgpersonnalization
: public itgtutor
{
public:
	itgpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the launching of the application  */
	}
	~itgpersonnalization( void )
	{
		/* place here the actions concerning this library
		to be realized at the ending of the application  */
	}
};


/*$END$*/
				  /********************************************/
				  /* do not modify anything belove this limit */
				  /*			  unless specified		   	  */
/******************************************************************************/

// 'static' by GNU C++.

static itgpersonnalization Tutor;

ttr_tutor &ITGTutor = Tutor;
