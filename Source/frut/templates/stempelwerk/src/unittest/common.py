import contextlib
import difflib
import filecmp
import json
import pathlib
import sys
import shutil

import pytest
from src.StempelWerk import StempelWerk


class TestCommon:
    # adapted from https://stackoverflow.com/a/42327075
    @contextlib.contextmanager
    def does_not_raise(self, exception):
        try:
            yield
        except exception:
            raise pytest.fail(f'raised unwanted exception {exception}')


    def assert_autocreated_paths(self, config, pre_check):
        root_dir = pathlib.Path(config['root_dir'])
        autocreated_paths = ['template_dir', 'output_dir']

        for dir_key in autocreated_paths:
            dir_path = root_dir / config[dir_key]
            # directories that should be autocreated do not exist yet
            if pre_check:
                assert not dir_path.is_dir(), \
                    f'directory "{dir_path}" already exists'
            # directories were autocreated
            else:
                assert dir_path.is_dir(), \
                    f'directory "{dir_path}" was not created'

    # ------------------------------------------------------------------------

    def modify_file(self, config, file_path):
        with file_path.open() as original_file:
            original_contents = original_file.readlines()

        # delete first line
        modified_contents = original_contents[1:]

        with file_path.open(mode='w') as modified_file:
            modified_file.writelines(modified_contents)

        # assert modification
        with pytest.raises(AssertionError):
            self.compare_directories(config)


    def update_file(self, input_path):
        output_path = pathlib.Path(
            str(input_path).replace('_updated', ''))

        shutil.copyfile(input_path, output_path)

    # ------------------------------------------------------------------------

    def create_config(self, custom_config, config_path,
                      common_path_separator=True):
        root_dir = config_path.parent
        if common_path_separator:
            root_dir = root_dir.as_posix()

        config = {
            'root_dir': str(root_dir),
            'template_dir': '10-templates',
            'output_dir': '20-output',
            # ----------------------------------------
            'included_suffixes': [
                '*.jinja'
            ],
            'stencil_dir_name': '',
            'create_directories': False,
            # ----------------------------------------
            'jinja_options': {
                'trim_blocks': True,
            },
            'jinja_extensions': [],
            'custom_modules': [],
            # ----------------------------------------
            'last_run_file': '.last_run',
            'marker_new_file': '### New file:',
            'marker_content': '### Content:',
            'newline': '\n'
        }

        # overwrite defaults with custom settings
        config.update(custom_config)

        json_string = json.dumps(config, ensure_ascii=False, indent=2)
        config_path.write_text(json_string)

        # load and parse stored settings
        actual_contents = config_path.read_text()
        actual_config = json.loads(actual_contents)

        return actual_config


    def compare_directories(self, config):
        root_dir = pathlib.Path(config['root_dir'])
        output_path = root_dir / config['output_dir']
        expected_base = root_dir / '30-expected'

        comparator = filecmp.dircmp(expected_base, output_path)

        assert not comparator.left_only, \
            f'these files were not generated: { comparator.left_only }'
        assert not comparator.right_only, \
            f'unexpected files were generated: { comparator.right_only }'

        assert not comparator.common_funny, \
            f'could not process these files: { comparator.common_funny }'
        assert not comparator.funny_files, \
            f'could not compare these files: { comparator.funny_files }'

        if comparator.diff_files:
            # only print first differing file
            differing_file_path = comparator.diff_files[0]

            path_expected = expected_base / differing_file_path
            with path_expected.open() as expected_file:
                expected_contents = expected_file.readlines()

            path_actual = output_path / differing_file_path
            with path_actual.open() as actual_file:
                actual_contents = actual_file.readlines()

            diff_result = difflib.unified_diff(
                expected_contents,
                actual_contents,
                fromfile=str(path_expected),
                tofile=str(path_actual))

            print('------------------------------------------------------')
            print()
            print('Difference between expected output and result:')
            print()
            sys.stdout.writelines(diff_result)
            print()

            assert False, 'Found differing files.'

    # ------------------------------------------------------------------------

    def init_stempelwerk(self, config_path=None, global_namespace=None,
                         process_only_modified=False):
        script_path = sys.argv[0]
        command_line_arguments = [script_path]

        if global_namespace:
            command_line_arguments.append('--globals')
            command_line_arguments.append(global_namespace)

        if process_only_modified:
            command_line_arguments.append('--only-modified')

        # allow testing for missing configuration on command line
        if config_path:
            command_line_arguments.append(str(config_path))

        parsed_args = StempelWerk.CommandLineParser(command_line_arguments)
        instance = StempelWerk(parsed_args.settings, parsed_args.verbosity)

        return instance, parsed_args


    def run(self, config_path=None, global_namespace=None,
            process_only_modified=False):
        instance, parsed_args = self.init_stempelwerk(
            config_path, global_namespace, process_only_modified)

        assert parsed_args.process_only_modified == \
            process_only_modified

        # "run_results" contains number of processed and saved files
        run_results = instance.render_all_templates(process_only_modified)
        run_results['instance'] = instance

        return run_results


    def run_with_config(self, custom_config, config_path,
                        global_namespace=None):
        config = self.create_config(custom_config, config_path)

        print('Configuration:')
        print(json.dumps(config, ensure_ascii=False, indent=2))

        run_results = self.run(config_path, global_namespace)
        run_results['configuration'] = config

        return run_results


    def run_and_compare(self, custom_config, config_path,
                        global_namespace=None):
        run_results = self.run_with_config(
            custom_config, config_path, global_namespace)

        self.compare_directories(
            run_results['configuration'])

        return run_results
