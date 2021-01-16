Matomo Reports
==============

This module adds reports for a Matomo installation to the Reports section of
your Backdrop CMS site and imports key traffic information from your Matomo
server.

Project homepage: https://github.com/backdrop-contrib/matomo_reports

For information about Matomo, see https://matomo.org/
For Matomo's FAQ, see https://matomo.org/faq/


Installation
------------

- Install this module using the official Backdrop CMS instructions at
  https://backdropcms.org/guide/modules.

- Visit the configuration page at Administration > Configuration > System >
  Matomo Reports (admin/config/system/matomo/reports) and enter the required
  information. (If you have the Matomo Analytics module installed, it is at
  Administration > Configuration > System > Matomo > Reports (the path is the
  same).)

- Add your Matomo reports token_auth either globally (at
  admin/config/system/matomo/reports) or individually (in each user profile)

- Enjoy ;)


Documentation
-------------

Reports
- This modules provides some of the Matomo reports directly in your Backdrop CMS
  site. Just follow the installation instructions and go to
  admin/config/system/matomo/reports.

Multisite
- Matomo Reports will show statistics of every site the token_auth has view
  permissions on the Matomo server. Administrators can limit access to only to
  some of the allowed sites.

Block
- A Matomo Report block is available for in-page statistics.

Matomo Analytics
- Matomo Analytics (https://backdropcms.org/project/matomo) is not a dependency,
  but a Matomo server is required to track your site.


Issues
------

Bugs and feature requests should be reported in the issue queue:
https://github.com/backdrop-contrib/matomo_reports/issues.


Current Maintainers
-------------------

- [Jason Flatt](https://github.com/oadaeh)
- You?


Credits
-------

- Ported to Backdrop CMS by [Jason Flatt](https://github.com/oadaeh)
- Originally written for Drupal by [xlyz](https://www.drupal.org/u/xlyz)
- Currently maintained for Drupal by [shelane](https://www.drupal.org/u/shelane)
- Drupal work sponsored by [Relinc](http://www.relinc.it)


License
-------

This project is GPL v2 software. See the LICENSE.txt file in this directory for
complete text.
