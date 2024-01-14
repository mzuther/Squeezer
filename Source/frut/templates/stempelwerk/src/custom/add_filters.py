from src.StempelWerk import StempelWerk


class CustomCode(StempelWerk.CustomCodeTemplate):
    def __init__(self, copy_of_settings, printer):
        super().__init__(copy_of_settings, printer)


    def _display_environment_change(self, new_environment, old_environment,
                                    display_type):
        differences = sorted(new_environment - old_environment)

        added = ['"' + difference + '"' for difference in differences]
        count = len(added)
        added = ', '.join(added)

        self.print_debug(f'  - Added { count } { display_type }: { added }.')


    def update_environment(self, jinja_environment):
        jinja_environment = super().update_environment(
            jinja_environment)


        def uppercase_first(string):
            return string[0].upper() + string[1:].lower()


        def add_exclamation_mark(string, is_spanish=False):
            if is_spanish:
                string = '¡' + string
            return string + '!'


        old_filters = set(jinja_environment.filters)

        jinja_environment.filters["upper_first"] = uppercase_first
        jinja_environment.filters["add_exclamation_mark"] = add_exclamation_mark

        new_filters = set(jinja_environment.filters)
        self._display_environment_change(new_filters, old_filters, 'filters')

        return jinja_environment
