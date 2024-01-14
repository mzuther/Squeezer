-- ========================================================================== --
-- Demonstration of creating multiple files from a single template
-- ========================================================================== --

-- NOTE: Please do not judge the power of StempelWerk by these
-- NOTE: rudimentary templates.  I have worked with it for years
-- NOTE: without coming even close to what is ultimately possible.

-- This file has been auto-generated from the following template:
-- stencils/20-sql_create_stored_procedure.sql.jinja


CREATE OR ALTER PROCEDURE DEMO.Procedure
AS
BEGIN
    BEGIN TRY
        BEGIN TRANSACTION;

        -- prepare data for merge
        WITH Load_Data AS
        (
            SELECT
                Key_Column_1,
                Key_Column_2,
                -- -------------------------------------------------- --
                String_Column,
                -- -------------------------------------------------- --
                -- este es un comentario automatico
                Integer_Column
            FROM
                DEMO.Source
        ),
        Converted_Value AS
        (
            SELECT
                Load_Data.*,
                -- -------------------------------------------------- --
                Numeric_Column =
                    2 *
                    3.14159 *
                    Integer_Column
            FROM
                Load_Data
        )
        -- merge data from tgt into src
        MERGE
            DEMO.Target AS tgt
        USING
            Converted_Value AS src
        ON
            tgt.Key_Column_1 = src.Key_Column_1 AND
            tgt.Key_Column_2 = src.Key_Column_2
        -- update rows that exist and differ between source and target table
        -- (https://stackoverflow.com/a/8948461)
        WHEN MATCHED AND EXISTS
            (
                SELECT
                    tgt.String_Column,
                    -- -------------------------------------------------- --
                    -- este es un comentario automatico
                    tgt.Integer_Column,
                    tgt.Numeric_Column

                EXCEPT

                SELECT
                    src.String_Column,
                    -- -------------------------------------------------- --
                    -- este es un comentario automatico
                    src.Integer_Column,
                    src.Numeric_Column
            )
            THEN UPDATE SET
                tgt.String_Column                  = src.String_Column,
                -- -------------------------------------------------- --
                tgt.Integer_Column                 = src.Integer_Column,
                tgt.Numeric_Column                 = src.Numeric_Column
        -- insert rows that are not in target table
        WHEN NOT MATCHED BY TARGET
            THEN INSERT
            (
                Key_Column_1,
                Key_Column_2,
                -- -------------------------------------------------- --
                String_Column,
                -- -------------------------------------------------- --
                Integer_Column,
                Numeric_Column
            )
            VALUES
            (
                src.Key_Column_1,
                src.Key_Column_2,
                -- -------------------------------------------------- --
                src.String_Column,
                -- -------------------------------------------------- --
                src.Integer_Column,
                src.Numeric_Column
            )
        -- delete rows that are not in source table
        WHEN NOT MATCHED BY SOURCE
            THEN DELETE;

        COMMIT TRANSACTION;
    END TRY

    BEGIN CATCH
        ROLLBACK TRANSACTION;

        -- propagate exception to caller
        THROW;
    END CATCH
END;
