#!C:\apps\Cygwin\bin\perl -w
#Modify the previous and the two next lines to fit to your configuration !
my $sabcmd = "/cygdrive/h/bin/sabcmd";	# Location of the 'sabcmd' command.
my $tmp = "/cygdrive/h/temp";	# Location of the directory where to put the temporary files.

# $Id$


use strict;
use CGI;
use Socket;
use IPC::Open2;
#use XML::Simple;
#use Data::Dumper;

sub file2string
{
	my $file;
	
	unless ( $#_ eq 0 ) {
		throw Error::Internal( "Incorrect number of argument" );
	}

	open(FILE, "$_[0]") || throw Error::IO( -IOfile => $_[0]);
	$file = join '', <FILE>;
	close(FILE);
	
	return $file;
}

sub sopen
{
    my ($indata, $command) = @_;

    IPC::Open3::open3(\*PIPEIN, \*PIPEOUT, \*PIPEERR, $command);
    print PIPEIN $indata;
    close PIPEIN;

    my $out = join '', <PIPEOUT>;
    my $err = join '', <PIPEERR>;
    
    return $out, $err;
}

sub sopenf
{
    my ($indata, $command) = @_;
    my $out = $tmp . "out" . time . int rand 9999;
    my $err = $tmp . "err" . time . int rand 9999;
    
    open( FILE, "| $command >$out 2>$err" );
    
    print FILE $indata;
    close FILE;
	
	my $outs =  file2string( $out );
	my $errs =  file2string( $err );
	
	unlink( $out );
	unlink( $err );
    
	return $outs, $errs;
}

#my $state = 'debug';
#my $state = 'text';
my $state = 'prod';

my $cgi = new CGI();

if ( defined $cgi->param( '_debug' ) ) {
	$state = 'debug';
}

if ( defined $cgi->param( '_xml' ) ) {
	$state = 'xml';
}

if ( ( $state eq 'debug' ) or ( $state eq 'text' ) ) {
	print "Content-type: text/plain\n\n";
}

if ( $state eq 'xml' ) {
	print "Content-type: text/plain\n\n";
}

if ( $state eq 'debug' ) {
	print "Sablotron command: $sabcmd\n";
	print "Query string : $ENV{QUERY_STRING}\n";
	print "STDIN : ", <STDIN>, "\n";
}

my @keys = $cgi->param;

if ( $state eq 'debug' ) {
	print "Keys : ".join( ', ', @keys )."\n";
}

my $target = $cgi->param( '_target' );

if ( $state eq 'debug' ) {
	print "Target : $target\n";
}

my $conf = file2string( $target.'.conf' );

if ( $state eq 'debug' ) {
	print "\n----- CONFIGURATION FILE BEGIN -----\n$conf----- CONFIGURATION FILE END -----\n\n";
}

my ($host,$service,$path,$default) = ( $conf =~ m|(.*)\:(.*)\n*(.*)\n*(.*)\n*| );

if ( $state eq 'debug' ) {
	print "Host : $host\tService : $service\tPath : $path\n";
}


my $proto = getprotobyname('tcp');
socket(Socket_Handle, PF_INET, SOCK_STREAM, $proto);
#my $port = getservbyname($service, 'tcp');
my $port = $service;
my $iaddr = gethostbyname( $host );
my $sin = sockaddr_in($port,$iaddr);
connect(Socket_Handle,$sin);


foreach my $key ( @keys ) {
	send( Socket_Handle, "$key\x00".$cgi->param($key)."\x00", 0 );
};

my $skin = $cgi->param( '_skin' );

if ( ( not defined $skin )
     or ( $skin eq '' ) ) {
	$skin = $default;
	send( Socket_Handle, "_skin\x00".$skin."\x00", 0 );
}

send (Socket_Handle, "\000",0);

my $buffer;
my $result = "";


recv Socket_Handle, $buffer, 1000, 0;

while ( length $buffer  ) {
	$result .= $buffer;
	recv Socket_Handle, $buffer, 1000, 0;
}

close Socket_Handle;

my ($context,$lang,$XML) = ( $result =~ m$(\w*)\s(\w*)((.|\n)*)$ );
my $XML = substr $result, length($context) + length( $lang ) + 1;

#my $XML = XMLin( $result );
#my $context = $XML->{ Content }->{ Action } . $XML->{ Content }->{ Target };

if ( ( length( $path ) ne 0 ) 
     and ( substr( $path, -1 ) ne '/' )
	 and ( substr( $path, -1 ) ne '\\' ) ) {
	$path .= '/';
}

my $XSL = $path.$skin."/".$lang."/".$context.".xsl";

if ( $state eq 'debug' ) {
	print "Context : $context\n";
	print "Language : $lang\n";
	print "XSL's file : $XSL\n\n";
	print "XML : $XML\n";
	print "Result : $result\n";
}

if ( $state eq 'prod' ) {
	print $cgi->header, sopen( $XML, "$sabcmd \"file://$XSL\"" );
}
if ( $state eq 'xml' ) {
 	print "$XML";
}

#if ( $state eq 'debug' ) {
#	print "----- XML BEGIN -----\n".Dumper(XMLin($XML))."----- XML END -----\n\n";
#}