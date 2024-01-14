# The story, all names, characters, and incidents portrayed in this test are
# fictitious. No identification with actual persons (living or deceased),
# places, buildings, and products is intended or should be inferred.
#
# In other words: I have great and helpful colleagues with a lot of humour. In
# order to make writing these tests more fun, I have used their (obfuscated)
# names, but all personality traits have been made up. I hope they have as much
# fun reading these tests as I had in writing them!

import pathlib

import pytest
from src.unittest.common import TestCommon

FIXTURE_DIR = pathlib.Path('src/unittest/') / 'tintin'


class TestTinTin(TestCommon):
    # Tin Tin lead a double life: he was both developer and a chairman of the
    # UTP (Union of Technology Purloiners). After "obtaining" StempelWerk from a
    # project, he noticed with horror that he had stolen free software - a faux
    # pas of the highest degree.
    #
    # Not much has changed - Tin Tin still leads a double life. He is now both
    # prison inmate and UTP outcast. To reduce his sentence (150 years) by a few
    # days, he agreed to test the more intricate parts of StempelWerk.
    #
    # As a thief who made the global news, he obviously starts by testing global
    # variables.
    @pytest.mark.datafiles(FIXTURE_DIR / '1_global_variables')
    def test_global_variables(self, datafiles):
        custom_config = {
            'stencil_dir_name': 'stencils',
        }

        global_namespace = '{"NO_cast": true}'

        # set up StempelWerk and execute full run
        config_path = datafiles / 'settings.json'
        self.run_and_compare(custom_config, config_path,
                             global_namespace=global_namespace)


    # After a year of intense testing, Tin Tin moved on to custom modules. He
    # wanted to call them "prison_cell" and "inmate_canteen", but the author of
    # StempelWerk put his foot down.
    @pytest.mark.datafiles(FIXTURE_DIR / '2_custom_module')
    def test_custom_module(self, datafiles):
        custom_config = {
            'stencil_dir_name': 'stencils',
            'custom_modules': [
                'src.custom.add_filters',
                'src.custom.add_tests',
            ],
        }

        # set up StempelWerk and execute full run
        config_path = datafiles / 'settings.json'
        self.run_and_compare(custom_config, config_path)


    # Only 148 years to go! At Tin Tin's current pace, he will run out of work
    # by the end of the century ...
    #
    # Anyway, he recently started testing Jinja extensions. We will keep you
    # posted on his progress. Don't expect news anytime soon, though.
    @pytest.mark.datafiles(FIXTURE_DIR / '3_jinja_extension')
    def test_jinja_extension(self, datafiles):
        custom_config = {
            'stencil_dir_name': 'stencils',
            'jinja_extensions': [
                'jinja2.ext.debug',
            ],
        }

        debug_path = datafiles / '20-output/Debug.txt'
        assert not debug_path.is_file()

        # set up StempelWerk and execute full run
        config_path = datafiles / 'settings.json'
        run_results = self.run_with_config(custom_config, config_path)

        assert debug_path.is_file()

        # assert that debug file has been created by this test
        debug_output = debug_path.read_text()
        print(debug_output)

        stencils = [
            '00-common_settings.sql.jinja',
            '10-sql_create_table.sql.jinja',
            '20-sql_create_stored_procedure.sql.jinja',
        ]

        for stencil in stencils:
            assert debug_output.find(stencil) != -1, \
                f'stencil "{stencil}" not found in debug output'

        # assert that output files are rendered correctly
        debug_path.unlink()
        self.compare_directories(run_results['configuration'])
