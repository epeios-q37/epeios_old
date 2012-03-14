/*
	'ui_ssn_vew.cpp' by Claude SIMON (http://zeusw.org/).

	 This file is part of 'eDemo' software.

    'eDemo' is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    'eDemo' is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with 'eDemo'.  If not, see <http://www.gnu.org/licenses/>.
*/

// $Id$

#include "ui_ssn_vew.h"

#include "ui.h"
#include "dmoinf.h"
#include "trunk.h"

using namespace ui_ssn_vew;
using namespace ui_base;

using nsxpcm::event__;

using trunk::trunk___;

struct country__ {
	const char
		*ISO,
		*Code,
		*Name;
};

country__ Countries_[] = {
	{ "4", "AF", "Afghanistan" },
	{ "8", "AL", "Albania" },
	{ "12", "DZ", "Algeria" },
	{ "16", "AS", "American Samoa" },
	{ "20", "AD", "Andorra" },
	{ "24", "AO", "Angola" },
	{ "660", "AI", "Anguilla" },
	{ "10", "AQ", "Antarctica" },
	{ "28", "AG", "Antigua And Barbuda" },
	{ "32", "AR", "Argentina" },
	{ "51", "AM", "Armenia" },
	{ "533", "AW", "Aruba" },
	{ "36", "AU", "Australia" },
	{ "40", "AT", "Austria" },
	{ "31", "AZ", "Azerbaijan" },
	{ "44", "BS", "Bahamas" },
	{ "48", "BH", "Bahrain" },
	{ "50", "BD", "Bangladesh" },
	{ "52", "BB", "Barbados" },
	{ "112", "BY", "Belarus" },
	{ "56", "BE", "Belgium" },
	{ "84", "BZ", "Belize" },
	{ "204", "BJ", "Benin" },
	{ "60", "BM", "Bermuda" },
	{ "64", "BT", "Bhutan" },
	{ "68", "BO", "Bolivia" },
	{ "70", "BA", "Bosnia And Herzegovina" },
	{ "72", "BW", "Botswana" },
	{ "74", "BV", "Bouvet Island" },
	{ "76", "BR", "Brazil" },
	{ "86", "IO", "British Indian Ocean Territory" },
	{ "96", "BN", "Brunei Darussalam" },
	{ "100", "BG", "Bulgaria" },
	{ "854", "BF", "Burkina Faso" },
	{ "108", "BI", "Burundi" },
	{ "116", "KH", "Cambodia" },
	{ "120", "CM", "Cameroon" },
	{ "124", "CA", "Canada" },
	{ "132", "CV", "Cape Verde" },
	{ "136", "KY", "Cayman Islands" },
	{ "140", "CF", "Central African Republic" },
	{ "148", "TD", "Chad" },
	{ "152", "CL", "Chile" },
	{ "156", "CN", "China" },
	{ "162", "CX", "Christmas Island" },
	{ "166", "CC", "Cocos (Keeling) Islands" },
	{ "170", "CO", "Colombia" },
	{ "174", "KM", "Comoros" },
	{ "178", "CG", "Congo" },
	{ "184", "CK", "Cook Islands" },
	{ "188", "CR", "Costa Rica" },
	{ "384", "CI", "Cote D'Ivoire" },
	{ "191", "HR", "Croatia (Local Name: Hrvatska)" },
	{ "192", "CU", "Cuba" },
	{ "196", "CY", "Cyprus" },
	{ "203", "CZ", "Czech Republic" },
	{ "208", "DK", "Denmark" },
	{ "262", "DJ", "Djibouti" },
	{ "212", "DM", "Dominica" },
	{ "214", "DO", "Dominican Republic" },
	{ "218", "EC", "Ecuador" },
	{ "818", "EG", "Egypt" },
	{ "222", "SV", "El Salvador" },
	{ "226", "GQ", "Equatorial Guinea" },
	{ "232", "ER", "Eritrea" },
	{ "233", "EE", "Estonia" },
	{ "210", "ET", "Ethiopia" },
	{ "238", "FK", "Falkland Islands (Malvinas)" },
	{ "234", "FO", "Faroe Islands" },
	{ "242", "FJ", "Fiji" },
	{ "246", "FI", "Finland" },
	{ "250", "FR", "France" },
	{ "249", "FX", "France, Metropolitan" },
	{ "254", "GF", "French Guiana" },
	{ "258", "PF", "French Polynesia" },
	{ "260", "TF", "French Southern Territories" },
	{ "266", "GA", "Gabon" },
	{ "270", "GM", "Gambia" },
	{ "268", "GE", "Georgia" },
	{ "276", "DE", "Germany" },
	{ "288", "GH", "Ghana" },
	{ "292", "GI", "Gibraltar" },
	{ "300", "GR", "Greece" },
	{ "304", "GL", "Greenland" },
	{ "308", "GD", "Grenada" },
	{ "312", "GP", "Guadeloupe" },
	{ "316", "GU", "Guam" },
	{ "320", "GT", "Guatemala" },
	{ "324", "GN", "Guinea" },
	{ "624", "GW", "Guinea-Bissau" },
	{ "328", "GY", "Guyana" },
	{ "332", "HT", "Haiti" },
	{ "334", "HM", "Heard Island & Mcdonald Islands" },
	{ "340", "HN", "Honduras" },
	{ "344", "HK", "Hong Kong" },
	{ "348", "HU", "Hungary" },
	{ "352", "IS", "Iceland" },
	{ "356", "IN", "India" },
	{ "360", "ID", "Indonesia" },
	{ "364", "IR", "Iran, Islamic Republic Of" },
	{ "368", "IQ", "Iraq" },
	{ "372", "IE", "Ireland" },
	{ "376", "IL", "Israel" },
	{ "380", "IT", "Italy" },
	{ "388", "JM", "Jamaica" },
	{ "392", "JP", "Japan" },
	{ "400", "JO", "Jordan" },
	{ "398", "KZ", "Kazakhstan" },
	{ "404", "KE", "Kenya" },
	{ "296", "KI", "Kiribati" },
	{ "408", "KP", "Korea, Democratic People'S Republic Of" },
	{ "410", "KR", "Korea, Republic Of" },
	{ "414", "KW", "Kuwait" },
	{ "417", "KG", "Kyrgyzstan" },
	{ "418", "LA", "Lao People'S Democratic Republic" },
	{ "428", "LV", "Latvia" },
	{ "422", "LB", "Lebanon" },
	{ "426", "LS", "Lesotho" },
	{ "430", "LR", "Liberia" },
	{ "434", "LY", "Libyan Arab Jamahiriya" },
	{ "438", "LI", "Liechtenstein" },
	{ "440", "LT", "Lithuania" },
	{ "442", "LU", "Luxembourg" },
	{ "446", "MO", "Macau" },
	{ "807", "MK", "Macedonia, The Former Yugoslav Republic Of" },
	{ "450", "MG", "Madagascar" },
	{ "454", "MW", "Malawi" },
	{ "458", "MY", "Malaysia" },
	{ "462", "MV", "Maldives" },
	{ "466", "ML", "Mali" },
	{ "470", "MT", "Malta" },
	{ "584", "MH", "Marshall Islands" },
	{ "474", "MQ", "Martinique" },
	{ "478", "MR", "Mauritania" },
	{ "480", "MU", "Mauritius" },
	{ "175", "YT", "Mayotte" },
	{ "484", "MX", "Mexico" },
	{ "583", "FM", "Micronesia, Federated States Of" },
	{ "498", "MD", "Moldova, Republic Of" },
	{ "492", "MC", "Monaco" },
	{ "496", "MN", "Mongolia" },
	{ "500", "MS", "Montserrat" },
	{ "504", "MA", "Morocco" },
	{ "508", "MZ", "Mozambique" },
	{ "104", "MM", "Myanmar" },
	{ "516", "NA", "Namibia" },
	{ "520", "NR", "Nauru" },
	{ "524", "NP", "Nepal" },
	{ "528", "NL", "Netherlands" },
	{ "530", "AN", "Netherlands Antilles" },
	{ "540", "NC", "New Caledonia" },
	{ "554", "NZ", "New Zealand" },
	{ "558", "NI", "Nicaragua" },
	{ "562", "NE", "Niger" },
	{ "566", "NG", "Nigeria" },
	{ "570", "NU", "Niue" },
	{ "574", "NF", "Norfolk Island" },
	{ "580", "MP", "Northern Mariana Islands" },
	{ "578", "NO", "Norway" },
	{ "512", "OM", "Oman" },
	{ "586", "PK", "Pakistan" },
	{ "585", "PW", "Palau" },
	{ "591", "PA", "Panama" },
	{ "598", "PG", "Papua New Guinea" },
	{ "600", "PY", "Paraguay" },
	{ "604", "PE", "Peru" },
	{ "608", "PH", "Philippines" },
	{ "612", "PN", "Pitcairn" },
	{ "616", "PL", "Poland" },
	{ "620", "PT", "Portugal" },
	{ "630", "PR", "Puerto Rico" },
	{ "634", "QA", "Qatar" },
	{ "638", "RE", "Reunion" },
	{ "642", "RO", "Romania" },
	{ "643", "RU", "Russian Federation" },
	{ "646", "RW", "Rwanda" },
	{ "654", "SH", "Saint Helena" },
	{ "659", "KN", "Saint Kitts And Nevis" },
	{ "662", "LC", "Saint Lucia" },
	{ "666", "PM", "Saint Pierre And Miquelon" },
	{ "670", "VC", "Saint Vincent And The Grenadines" },
	{ "882", "WS", "Samoa" },
	{ "674", "SM", "San Marino" },
	{ "678", "ST", "Sao Tome And Principe" },
	{ "682", "SA", "Saudi Arabia" },
	{ "686", "SN", "Senegal" },
	{ "690", "SC", "Seychelles" },
	{ "694", "SL", "Sierra Leone" },
	{ "702", "SG", "Singapore" },
	{ "703", "SK", "Slovakia (Slovak Republic)" },
	{ "705", "SI", "Slovenia" },
	{ "90", "SB", "Solomon Islands" },
	{ "706", "SO", "Somalia" },
	{ "710", "ZA", "South Africa" },
	{ "724", "ES", "Spain" },
	{ "144", "LK", "Sri Lanka" },
	{ "736", "SD", "Sudan" },
	{ "740", "SR", "Suriname" },
	{ "744", "SJ", "Svalbard And Jan Mayen Islands" },
	{ "748", "SZ", "Swaziland" },
	{ "752", "SE", "Sweden" },
	{ "756", "CH", "Switzerland" },
	{ "760", "SY", "Syrian Arab Republic" },
	{ "158", "TW", "Taiwan, Province Of China" },
	{ "762", "TJ", "Tajikistan" },
	{ "834", "TZ", "Tanzania, United Republic Of" },
	{ "764", "TH", "Thailand" },
	{ "768", "TG", "Togo" },
	{ "772", "TK", "Tokelau" },
	{ "776", "TO", "Tonga" },
	{ "780", "TT", "Trinidad And Tobago" },
	{ "788", "TN", "Tunisia" },
	{ "792", "TR", "Turkey" },
	{ "795", "TM", "Turkmenistan" },
	{ "796", "TC", "Turks And Caicos Islands" },
	{ "798", "TV", "Tuvalu" },
	{ "800", "UG", "Uganda" },
	{ "804", "UA", "Ukraine" },
	{ "784", "AE", "United Arab Emirates" },
	{ "826", "GB", "United Kingdom" },
	{ "840", "US", "United States" },
	{ "581", "UM", "United States Minor Outlying Islands" },
	{ "858", "UY", "Uruguay" },
	{ "860", "UZ", "Uzbekistan" },
	{ "548", "VU", "Vanuatu" },
	{ "336", "VA", "Vatican City State (Holy See)" },
	{ "862", "VE", "Venezuela" },
	{ "704", "VN", "Viet Nam" },
	{ "92", "VG", "Virgin Islands (British)" },
	{ "850", "VI", "Virgin Islands (U.S.)" },
	{ "876", "WF", "Wallis And Futuna Islands" },
	{ "732", "EH", "Western Sahara" },
	{ "887", "YE", "Yemen" },
	{ "891", "YU", "Yugoslavia" },
	{ "180", "ZR", "Zaire" },
	{ "894", "ZM", "Zambia" },
	{ "716", "ZW", "Zimbabwe" },
};

void ui_ssn_vew::get_directory_eh__::NSXPCMOnEvent( nsxpcm::event__ Event )
{
	Trunk().GetDirectoryName();
}

void ui_ssn_vew::update_eh__::NSXPCMOnEvent( nsxpcm::event__ Event )
{
	Trunk().UI().SessionView.Update();
}

/* Registrations */

/* 'broadcaster's */

#define R( name ) Trunk, Broadcasters.name.Init( Trunk, Trunk.UI().SessionView.Window(), #name );

static void Register_(
	trunk___ &Trunk,
	session_view__::broadcasters__ &Broadcasters,
	nsIDOMWindow *Window )
{
}

#undef R

# define R( name )\
	EventHandlers.name.Init( Trunk );\
	nsxpcm::Attach( Trunk.UI().SessionView.Document(), #name, EventHandlers.name );
static void Register_(
	trunk___ &Trunk,
	session_view__::event_handlers__ &EventHandlers,
	nsIDOMWindow *Window )
{
	R( ehGetDirectory );
	R( ehUpdate );
}

#undef R

static inline bso::ulong__ GetTotalAmount_( void )
{
	return sizeof( Countries_ )/sizeof( *Countries_ );
}

static inline bso::bool__ Match_( 
	const char *Pattern,
	const char *Candidate )
{
	if ( strlen( Pattern ) > strlen( Candidate ) )
		return false;

	return strncmp( Pattern, Candidate, strlen( Pattern ) ) == 0;
}

static bso::ulong__ GetFirstAndAmount_(
	const char *Pattern,
	bso::ulong__ &First )
{
	bso::ulong__ Limit = GetTotalAmount_() - 1;
	bso::ulong__ Amount = 0;
	bso::ulong__ Length = strlen( Pattern );

	while ( ( First < Limit ) && !Match_( Pattern, Countries_[First].Name ) )
		First++;

	if ( First != Limit )
		while ( ( ( First + Amount ) < Limit ) && Match_( Pattern, Countries_[First+Amount].Name ) )
			Amount ++;

	return Amount;
}

class _callback__
: public xulfbs::autocomplete_textbox_callback__
{
protected:
	virtual void XULFBSGetValue(
		const str::string_ &Pattern,
		bso::ulong__ Index,
		str::string_ &Value )
	{
	ERRProlog
		bso::ulong__ First = 0;
		STR_BUFFER___ Buffer;
	ERRBegin
		GetFirstAndAmount_( Pattern.Convert( Buffer ), First );

		Value.Append( Countries_[First+Index].Name );
	ERRErr
	ERREnd
	ERREpilog
	}
	virtual void XULFBSGetComment(
		const str::string_ &Pattern,
		bso::ulong__ Index,
		str::string_ &Comment )
	{
	ERRProlog
		bso::ulong__ First = 0;
		STR_BUFFER___ Buffer;
	ERRBegin
		GetFirstAndAmount_( Pattern.Convert( Buffer ), First );

		Comment.Append( Countries_[First+Index].Code );
	ERRErr
	ERREnd
	ERREpilog
	}
	virtual bso::ulong__ XULFBSGetMatchingCount( const str::string_ &Pattern )
	{
		bso::ulong__ Amount = 0;
	ERRProlog
		STR_BUFFER___ Buffer;
		bso::ulong__ First = 0;
	ERRBegin
		Amount = GetFirstAndAmount_( Pattern.Convert( Buffer ), First );
	ERRErr
	ERREnd
	ERREpilog
		return Amount;
	}
	virtual void XULFBSOnTextEntered( nsIDOMElement *Element )
	{
		Element = Element;
	}
	virtual void XULFBSOnTextReverted( nsIDOMElement *Element )
	{
		Element = Element;
	}
} Callback_;

#define R( name ) Widgets.name.Init( Trunk, Trunk.UI().SessionView.Window(), #name );

static void Register_(
	trunk___ &Trunk,
	session_view__::widgets__ &Widgets,
	nsIDOMWindow *Window )
{
	R( txbDirectorySymbolicName );
	R( txbDirectory );

	Callback_.Init();
	Widgets.txbAutocomplete.Init( Callback_, Trunk, Window, "txbAutocomplete" );

	R( ckbHistory );
	R( ckbComment );
	R( ckbCompleteDefaultIndex );
	R( ckbForceComplete );
	R( ckbHighlightNonMatches );
	R( ckbIgnoreBlurWhileSearching );
}

void ui_ssn_vew::session_view__::Init(
	nsIDOMWindow *Window,
	trunk::trunk___ &Trunk )
{
	_session_view__::Init( Window, Trunk );
	_Trunk = &Trunk;
}

#define C( name )		Widgets.txbAutocomplete.Enable##name( Widgets.ckb##name.IsChecked() );


void ui_ssn_vew::session_view__::Update( void )
{
	_session_view__::Update();
	Grid.Update();

	C( History );
	C( Comment );
//	C( CompleteDefaultIndex );
	C( ForceComplete );
	C( HighlightNonMatches );
	C( IgnoreBlurWhileSearching );
}

void ui_ssn_vew::RegisterSessionViewUI(
	trunk___ &Trunk,
	nsIDOMWindow *Window )
{
	Trunk.UI().SessionView.Init( Window, Trunk );

	xulfsv::RegisterSessionViewUI( Trunk, Window );

	Register_( Trunk, Trunk.UI().SessionView.Broadcasters, Window );
	Register_( Trunk, Trunk.UI().SessionView.EventHandlers, Window );
	Register_( Trunk, Trunk.UI().SessionView.Widgets, Window );

	nsxpcm::PatchOverallBadCommandBehavior( Trunk.UI().SessionView.Document() );
}
