# Change Log

_All notable changes to this project will be documented in this
file. This change log follows the conventions of
[keepachangelog.com]._

_I tend to not use
[semantic versioning](https://semver.org/), but we will see what
happens._

<!--- ---------------------------------------------------------------------- -->

## [Unreleased]

### Changed

<!--- ---------------------------------------------------------------------- -->

## [1.0.1] - 2024-01-12

### Security
- update Jinja2 dependency to fix security vulnerability

### Changed
- move poetry calls to separate Bash script
- refactor code
- update dependencies
- update documentation

<!--- ---------------------------------------------------------------------- -->

## [1.0.0] - 2023-07-24

- This is the first stable release (although I have been using StempelWerk professionally for over a year now).

### Added
- use poetry to create virtual environment
- add common scripts for PowerShell

### Fixed
- fix: unit test erroneously finds stencil on Windows
  - Windows is not case-sensitive. It really should be, but it is not. Sigh.

<!--- ---------------------------------------------------------------------- -->

[keepachangelog.com]: http://keepachangelog.com/
[unreleased]: https://github.com/mzuther/StempelWerk/tree/develop
[1.0.0]: https://github.com/mzuther/StempelWerk/commits/v1.0.0
