Matomo Reports
--------------------------------------------------------------------------------

Maintainer:  xlyz, xlyz@tiscali.it, shelane

This module adds a Matomo reports section and imports key traffic information
from your Matomo server.

Project homepage: http://drupal.org/project/piwik_reports

Issues: https://drupal.org/project/issues/piwik_reports

Sponsored by Relinc: http://www.relinc.it


Installation
------------

 * Copy the whole matomo_reports directory to your modules directory
   (e.g., DRUPAL_ROOT/sites/all/modules) and activate it in the modules page

 * Add your Matomo reports token_auth either globally (at
   admin/config/system/matomo) or individually (in each user profile)

 * Enjoy ;)
 

Documentation
-------------

Reports
This modules provides some of the matomo reports directly in your Drupal
site. Just follow the installation instructions and go to 
admin/reports/matomo_reports.

Multisite
Matomo reports will show statistics of every site the token_auth has view
permissions on the matomo server. Administrators can limit access to only to
some of the allowed sites.

Block
A matomo page report block is available for in-page statistics.

Matomo Matomo Web Analytics
Matomo Matomo Web Analytics (https://drupal.org/project/matomo) is not a dependency
any more, but matomo is required to track your site.
