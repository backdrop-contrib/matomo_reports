
--------------------------------------------------------------------------------
Piwik Reports
--------------------------------------------------------------------------------

Maintainer:  xlyz, xlyz@tiscali.it, shelane

This module adds a piwik reports section and imports key traffic information 
from your piwik server.

Project homepage: http://drupal.org/project/piwik_reports

Issues: https://drupal.org/project/issues/piwik_reports

Sponsored by Relinc: http://www.relinc.it


Installation
------------

 * Copy the whole piwik_reports directory to your modules directory
   (e.g., DRUPAL_ROOT/sites/all/modules) and activate it in the modules page

 * Add your Piwik reports token_auth either globally (at 
   admin/config/system/piwik) or individually (in each user profile)

 * Enjoy ;)
 

Documentation
-------------

Reports
This modules provides some of the piwik reports directly in your Drupal
site. Just follow the installation instructions and go to 
admin/reports/piwik_reports.

Multisite
Piwik reports will show statistics of every site the token_auth has view
permissions on the piwik server. Administrators can limit access to only to 
some of the allowed sites.

Block
A piwik page report block is available for in-page statistics.

Piwik Piwik Web Analytics
Piwik Piwik Web Analytics (https://drupal.org/project/piwik) is not a dependency
any more, but piwik is required to track your site.

