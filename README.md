# Islandora Find & Replace

## Introduction

An administrative user interface to perform find & replace on datastream text.

## Requirements

This module requires the following modules/libraries:

* [Islandora](https://github.com/islandora/islandora)
* [Tuque](https://github.com/islandora/tuque)
* [Islandora Pretty Text Diff](https://github.com/contentmath/islandora_pretty_text_diff) (not a dependency but highly recommended)

## Installation

Follow the typical Drupal module installation instructions [here](https://drupal.org/documentation/install/modules-themes/modules-7).

## Documentation

Use the find form to search for the text by content model (required), collection (optional), and created or modified date (optional). Then the datastream ID is selected for the updates to apply to, the search text, and the replacement text. Search text and replacement text are case sensitive. If the search text should be completely removed rather than replaced, enter "(empty)" in the Replacement text field.

The module will then start a batch process in the browser to find all of the objects matching the conditions. This operation may take a few minutes if a large amount of objects are targeted by the search conditions. This can be mitigated by making your search more specific.

The next step in the form allows the user to select which objects (or all) should be updated. If the Islandora Pretty Text Diff module is enabled (highly recommended), a link to a preview diff of the changes will be provided. Submitting this form will kick off the update batch process. After completion, a log will be displayed of successful updates.

**Please note that this module has the potential to edit tags and make datastreams invalid if used incorrectly, so it is recommended to test on sample data first and always ensure backups are up-to-date. The module will not operate on system critical datatstreams like RELS-EXT and POLICY.**

The find and replace operation respects Islandora's permissions, namespace restrictions, and XACML access to an object. The module does not update derivative datastreams, like the case of MODS transformations to Dublin Core. The derivative datastreams can be targeted with their own find and replace operation if this is problematic. Currently, this module does not have the functionality to update object labels.

A list of all completed Islandora Find & Replace operations can be found by visiting the admin/islandora/tools/find-replace/log URL on your Islandora installation.

## Troubleshooting/Issues

We welcome issues in our [issue queue](https://github.com/mnylc/islandora_find_replace/issues).

## Maintainers/Sponsors

Current maintainers:

* [Diego Pino](https://github.com/DiegoPino/)

## License

[GPLv3](http://www.gnu.org/licenses/gpl-3.0.txt)

## Acknowledgments

* [@mitchmac](https://github.com/mitchmac) is responsible for the bulk of the Find & Replace code
* Forked from [@cuhk-library](https://github.com/cuhk-library/islandora_find_replace)
* Updated at ICG hack/doc at Williams College, May 2017
