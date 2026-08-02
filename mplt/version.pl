#!/usr/bin/perl -w
use strict;
use warnings;
#
# declare local variables
#
my $pgm = "plt";
my $PGM = uc $pgm;
my $n;
my @infile;
my $ver;
my $vdt;
my $i;
#
# get version number from version.h
#
$n = "../tplt/version.h";
open FILE,$n or die "can't open ".$n;
@infile = <FILE>;
foreach(@infile)
{
    if($_ =~ /#define\sREV.*/) {
	($ver) = $_ =~ /.*(\d\.\d\d[a-z]*), .*"/;
	($vdt) = $_ =~ /.*\d\.\d\d[a-z]*, (.*)"/;
    }
}
close (FILE);
print "version is $ver on $vdt\n";

# update version in MacPlot/Info.plist
#
$n = "MacPlot/Info.plist";
open FILE,$n or die "can't open ".$n;
@infile = <FILE>;
$i = 0;
foreach(@infile)
{
    if($_ =~ /Version.*Date.*[0-9]/) {
	$infile[$i] =~ s//Version: $ver, $vdt/;
    }
    $i ++;
} 
close FILE;
open FILE,">".$n;
print FILE @infile;
close FILE;

# update version in package/readme.txt
#
$n = "package/readme.txt";
open FILE,$n or die "can't open ".$n;
@infile = <FILE>;
$i = 0;
foreach(@infile)
{
    if($_ =~ /version.*/) {
	$infile[$i] =~ s//version $ver, $vdt/;
    }
    $i ++;
} 
close FILE;
open FILE,">".$n;
print FILE @infile;
close FILE;


# update version in MacPlot.xcodeproj/project.pbxproj
#
$n = "MacPlot.xcodeproj/project.pbxproj";
open FILE,$n or die "can't open ".$n;
@infile = <FILE>;
$i = 0;
foreach(@infile)
{
    if($_ =~ /_VERSION/) {
	$infile[$i] =~ s/".*"/"$ver, $vdt"/;
    }
    $i ++;
} 
close FILE;
open FILE,">".$n;
print FILE @infile;
close FILE;

