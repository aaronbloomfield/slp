<?php

/**
 * This utility is designed to check that the files in the docs/
 * directory in the repo are what is required, as per the course
 * conclusions slide set.
 *
 * Run this on the command line by changing to the docs/ directory in
 * your github repo and running:
 *
 * php check-docs.php
 *
 * You may have to install the php-cli package if it doesn't work.
 */

// the list of expected documents
$docs = array (
	       "code-coverage-excuses.*" => "code coverage excuses (optional)", 
	       "coverage/" => "coverage/ directory", 
	       "coverage/index.html" => "coverage/index.html", 
	       "deployment-plan.*" => "deployment plan source", 
	       "deployment-plan.pdf" => "deployment plan PDF", 
	       "final-presentation.*" => "final presentation source", 
	       "final-presentation.pdf" => "final presentation PDF", 
	       "installation-instructions.*" => "installation instructions source", 
	       "installation-instructions.pdf" => "installation instructions PDF", 
	       "master-test-plan.*" => "master test plan source", 
	       "master-test-plan.pdf" => "master test plan PDF", 
	       "one-slide.pptx" => "one slide version", 
	       "programming-reference.pdf" => "programming refernce PDF", 
	       "programming-reference.*" => "programming reference source", 
	       "readme.md" => "readme", 
	       "requirements-document.pdf" => "requirements document PDF", 
	       "requirements-document.*" => "requirements document source", 
	       "screenshot*.png" => "screenshots", 
	       "specific-test-plans.pdf" => "specific test plan PDF", 
	       "specific-test-plans.*" => "specific test plan source", 
	       "technical-report.pdf" => "technical report PDF", 
	       "technical-report.*" => "technical report source", 
	       "user-manual.pdf" => "user manual PDF", 
	       "user-manual.*" => "user manual source", 
	       );

$allowunderscores = false; // do we allow underscores in file names? currently, no.

// get the files in the currect directory
$files = split("\n",trim(`ls -F . coverage/index.html 2> /dev/null | sort`));

// Go through each file found in the directory
foreach ( array_keys($files) as $i ) {

  $file = $files[$i];
  if ( $allowunderscores )
    $fileus = str_replace("_","-",$file); // allow underscores in the file name
  else
    $fileus = $file;

  // remove empty entries
  if ( ($file == "") || ($file == ".:") )
    unset($files[$i]);

  // if it's a PDF file, an HTML file, a powerpoint file, or a
  // directory, and it's found, then remove it from the two lists
  if ( ((substr($file,-4) == ".pdf") ||
	(substr($file,-3) == ".md") ||
	(substr($file,-1) == "/") ||
	(substr($file,-5) == ".pptx") ||
	(substr($file,-5) == ".html")) &&
       (isset($docs[$file]) || isset($docs[$fileus])) ) {
    unset($docs[$fileus]);
    unset($files[$i]);
  }

}

// Go through each file expected
foreach ( array_keys($docs) as $doc ) {
  if ( $allowunderscores )
    $docus = str_replace("-","_",$doc);
  else
    $docus = $doc;

  // if the file name ends with a '.*', then we don't care about the
  // extension (and it could even be a directory)
  if ( substr($doc,-2) == ".*" ) {
    $filenamebase = substr($doc,0,strlen($doc)-1);
    if ( $allowunderscores )
      $filenamebaseus = substr($docus,0,strlen($docus)-1); // allow underscores in the file name
    else
      $filenamebaseus = $filenamebase;

    // see if anything in the directory matches that file name
    foreach ( array_keys($files) as $i ) {
      if ( !strncmp($files[$i],$filenamebase,strlen($filenamebase)) ||
	   !strncmp($files[$i],$filenamebaseus,strlen($filenamebaseus)) ) {
	// found a match!
	unset($files[$i]);
	unset($docs[$doc]);
	break;
      }
    }

    // see if there is a directory of that name
    $dirname = substr($doc,0,strlen($doc)-2) . "/";
    if ( $allowunderscores )
      $dirnameus = substr($docus,0,strlen($docus)-2) . "/";
    else
      $dirnameus = $dirname;
    if ( (array_search($dirname,$files) != FALSE) ||
	 (array_search($dirnameus,$files) != FALSE) ) {
      unset($docs[$doc]);
      $i = array_search($dirname,$files);
      if ( $i != FALSE )
	unset($files[$i]);
      $i = array_search($dirnameus,$files);
      if ( $i != FALSE )
	unset($files[$i]);
    }

  }

}

// handle screen shots separately
if ( isset($docs["screenshot*.png"]) ) {
  $screenshots = split("\n",trim(`ls screenshot* 2> /dev/null | sort`));
  foreach ( $screenshots as $file )
    if ( (substr($file,-4) == ".jpg") ||
	 (substr($file,-4) == ".png") ) {
      unset($docs["screenshot*.png"]);
      $i = array_search($file,$files);
      if ( $i != FALSE )
	unset($files[$i]);
    }
}

// reindex the files array
$files = array_values($files);

echo "The following expected files were not found:\n";
print_r($docs);

echo "\nThe following extra files were found (this is not necessarily bad, btw):\n";
print_r($files);


?>
