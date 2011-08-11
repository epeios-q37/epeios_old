/*
	Header for the 'common' module by Claude SIMON (http://zeusw.org/epeios/contact.html).
	Part of the 'erism' tool.
	Copyright (C) 2011 by Claude SIMON (http://zeusw.org/epeios/contact.html).

	This file is part of the Epeios project (http://zeusw.org/epeios/).

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 3
	of the License, or (at your option) any later version.
 
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, go to http://www.fsf.org/
	or write to the:
  
         	         Free Software Foundation, Inc.,
           59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*/
// $Id$


#ifndef COMMON__INC
# define COMMON__INC

# include "erism.h"
# include "registry.h"
# include "mscrmi.h"
# include "mscmdd.h"

# include "lcl.h"
# include "cio.h"

namespace common {
	extern cio::cerr___ cerr;
	extern cio::cout___ cout;
	extern cio::cin___ cin;

	extern lcl::rack__ LocaleRack;
	

	using registry::value_;
	E_AUTO( value )

	enum message__ {
		mDevicesCommandDescription,
		mIdentifyCommandDescription,
		mRetrieveCommandDescription,
		mWriteCommandDescription,
		mRandomCommandDescription,
		mDetailCommandDescription,

		mDInOptionDescription,
		mDOutOptionDescription,

		mTargetArgumentDescription,
		mSettingsArgumentDescription,
		mAddressRangeArgumentDescription,
		mValueRangeArgumentDescription,

		mNoMIDIOutDevicesAvailable,
		mAvailableMIDIOutDevices,

		mNoMIDIInDevicesAvailable,
		mAvailableMIDIInDevices,

		mDeviceModel,

		m_amount,
		m_Undefined
	};

#	define COMMON__MESSAGE_AMOUNT	17

	const char *Label( message__ Message );

	inline const str::string_ &GetTranslation(
		message__ Message,
		str::string_ &Translation )
	{
		LocaleRack.GetTranslation( Label( Message ), "", Translation );

		return Translation;
	}

	void Display(
		message__ Message,
		... );

	enum error__ {
		eMissingDOutID,
		eMissingDInID,
		eBadDOutID,
		eBadDInID,

		eUnableToOpenMIDIDevices,

		eMIDIImplementationsFileNameNotDefined,
		eMIDIImplementationFileError,

		eBadDeviceID,
		eUnableToIdentifyDevice,

		eErrorParsingConfigurationFile,
		eErrorParsingLocaleFile,

		eMissingSettingsFileName,
		eSettingsFileError,

		eUnableToOpenFile,

		eTransmissionError,

		eBadAddressRange,
		eBadValueRange,
		eUnableToFindParameterForAddress,

		e_amount,
		e_Undefined
	};

# define COMMON__ERROR_AMOUNT	18

	const char *Label( error__ Error );

	inline const str::string_ &GetTranslation(
		error__ Error,
		str::string_ &Translation )
	{
		LocaleRack.GetTranslation( Label( Error ), "", Translation );

		return Translation;
	}

	void Report(
		error__ Error,
		... );

	void Initialize( void );
	void Release();

	void GetImplementations( mscrmi::midi_implementations_ &Implementations );

	mscrmi::device_id__ GetDeviceID( const char *RawDeviceID );

	void Normalize(
		const char *RawDeviceID,
		const char *RawDIn,
		const char *RawDOut,
		mscrmi::device_id__ &DeviceID,
		int &DIn,
		int &DOut );

	void CreateBackupFile( const char *FileName );

	void RecoverBackupFile( const char *FileName );

	inline flw::ioflow__ &OpenDevices(
		int DIn,
		int DOut,
		mscmdd::midi_ioflow___ &Flow )
	{
		mscmdd::status__ Status = Flow.Init( DIn, DOut, err::hUserDefined );

		if ( Status != mscmdd::sOK )
			Report( eUnableToOpenMIDIDevices, &Status );

		return Flow;
	}

	inline void GetIdentity( 
		mscrmi::device_id__ DeviceID,
		const mscrmi::midi_implementation_ &Implementation,
		mscrmi::identity__ &Identity )
	{
		mscrmi::Set( DeviceID, Implementation, Identity );
	}

	const mscrmi::device_family__ &GetDeviceFamilyAndSoftwareRevision(
		mscrmi::device_id__ DeviceID,
		flw::ioflow__ &Flow,
		mscrmi::device_family__ &DeviceFamily,
		mscrmi::software_revision__ &SoftwareRevision );

	const mscrmi::device_family__ &GetDeviceFamilyAndSoftwareRevision(
		mscrmi::device_id__ DeviceID,
		int DIn,
		int DOut,
		mscrmi::device_family__ &DeviceFamily,
		mscrmi::software_revision__ &SoftwareRevision );

	epeios::row__ Identify(
		mscrmi::device_id__ DeviceID,
		flw::ioflow__ &Flow,
		const mscrmi::midi_implementations_ &Implementations,
		mscrmi::device_family__ &DeviceFamily,
		mscrmi::software_revision__ &SoftwareRevision );

	epeios::row__ Identify(
		mscrmi::device_id__ DeviceID,
		int DIn,
		int DOut,
		const mscrmi::midi_implementations_ &Implementations,
		mscrmi::device_family__ &DeviceFamily,
		mscrmi::software_revision__ &SoftwareRevision );

	 const mscrmi::midi_implementation_ &GetImplementation(
		mscrmi::device_id__ DeviceID,
		flw::ioflow__ &Flow,
		mscrmi::midi_implementation_ &Implementation );

	 const mscrmi::midi_implementation_ &GetImplementation(
		mscrmi::device_id__ DeviceID,
		int DIn,
		int DOut,
		mscrmi::midi_implementation_ &Implementation );
}

#endif