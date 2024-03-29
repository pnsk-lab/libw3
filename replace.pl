#!/usr/bin/env perl
# $Id$
# To put LICENSE into the source code

use IO::Handle;

my $str = "";
while(<STDIN>){
	$str = $str . $_;
}

my $rep = 74;

my $repl = "/* " . ("-" x $rep) . " */\n";

my $io = IO::Handle->new();

if(open $io, '<', "HEADER"){
	while(<$io>){
		my @list = $_ =~ /.{1,$rep}/g;
		for my $s (@list){
			$repl = $repl . "/* $s" . (" " x ($rep - length($s))) . " */\n";
		}
	}
	$repl = $repl . "/* " . ("-" x $rep) . " */\n";
	$io->close;
}

$io = IO::Handle->new();

open $io, '<', "LICENSE" or die "$!";

while(<$io>){
	my @list = $_ =~ /.{1,$rep}/g;
	for my $s (@list){
		$repl = $repl . "/* $s" . (" " x ($rep - length($s))) . " */\n";
	}
}

$io->close;

$repl = $repl . "/* " . ("-" x $rep) . " */\n";

$str =~ s/\/\* START LICENSE \*\/\n(.+\n)?\/\* END LICENSE \*\//\/\* START LICENSE \*\/\n$repl\/\* END LICENSE \*\//gs;
print $str;
