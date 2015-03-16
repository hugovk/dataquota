[Change log](http://dataquota.googlecode.com/svn/trunk/group/changelog.txt)


---


1.36 (6th June 2011) -
  * Choose daily or monthly quota.
  * Show this changelog during installation.

1.35 (10th April 2011) - Updated German.

1.34 (19th January 2011) -
  * Added touch feedback for  Symbian`^`1 and Symbian`^`3 phones.
  * Added Help and "More apps" menu options.
  * Updated French.

1.33 (25th November 2010) - Added Latvian.

1.32 (25th November 2010) -
  * Added US English, though it's just reusing UK English at the moment.
  * Added support for Symbian^3 devices.
  * Added touch support: if you press the data or date bars, it will let you enter a new value; touch elsewhere to refresh.
  * Fixed a touch screen bug: redraw after editing the values from the options menu.

1.31 (21st July 2010) - Added Azerbaijani (choose "unknown language").

1.30 (19th July 2010) - Added Hebrew properly this time, and display it right-to-left.

1.29 (1st June 2010) - Added Dutch and Lithuanian.

1.28 - Fixed an overflow error caused when more than 2GB of data has been sent or received.

1.27 - Added Estonian.

1.26 - Added Hebrew and updated Polish.

1.25 - Added Ukrainian.

1.24 - Added Polish and updated Italian.

1.23 - Fixed missing application icon on S60 3rd Edition devices (e.g. Nokia E62, E70, N71). Verified with E70 using RDA. The application is built using an S60 3.2 SDK, and S60 3.0 devices can't read the compressed MIF files it builds. The fix is to include the /X switch for mifconv. This is a known issue for the 3.1 SDK that affects this subsequent SDK:
http://wiki.forum.nokia.com/index.php/KIS000531_-_Compatibility_problem_with_binary-encoded_SVG_images

1.22 - Updated the certificate used for self-signing. The previous was only valid for one year and expired on 11th January 2010. The new one is valid for 10 years and expires 14th January 2020.

1.21 - Added Korean.

1.20 - Added Greek.

1.19 - Added French.

1.18 - Russian and Turkish fixed.

1.17(0) - Added Italian and Russian.

1.16(0) - Added Hungarian and updated German.

1.15(0) - Added Serbian, and http://twitter.com/DataQuota to the About box.

1.14(0) - Added Finnish, and fixed non-Latin character encoding ([issue 14](https://code.google.com/p/dataquota/issues/detail?id=14)).

1.00(12) - Added Swedish and Chinese PRC.

0.1.11 - Added Slovak, and put the date and verison in the navipane.

0.1.10 - Added Spanish.

0.1.9 - Added Brazilian Portuguese and Czech.

0.1.8 - Added German.

0.1.7 - Added Romanian, and moved Refresh from the Options menu to the right softkey.

0.1.6 - Fixed [issue 6](https://code.google.com/p/dataquota/issues/detail?id=6): Setting quota to zero stops the application working.

0.1.5 - Fixed [issue 3](https://code.google.com/p/dataquota/issues/detail?id=3): bug with large quota.

0.1.4 - Added Turkish.

0.1.3 - Fix for the first part of [issue 1](https://code.google.com/p/dataquota/issues/detail?id=1) -- edit the billing day (i.e. day zero) in
case your quota isn't reset on the first of the month.

0.1.2 - Self-signed so you don't need to use Symbian Signed Online.

0.1.1 - Draw dotted lines to show the other bar's position. If you've used more data than the quota allows, draw the data bar and text in red. "Enter quota" -> "Edit quota".

0.0.1 - First version released.