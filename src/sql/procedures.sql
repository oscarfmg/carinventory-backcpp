create or replace procedure public.insert_car(
    IN model character varying,
    IN brand character varying,
    IN kilometers integer,
    IN price character varying,
    IN description character varying default null,
    IN year integer default null
) 
language 'plpgsql'
as $BODY$
declare
begin
 insert into cars(model, brand, kilometers, price, description, year)
  values (model, brand, kilometers, price, description, year);
 commit;
 exception when others then
 	rollback;
	raise;
end;
$BODY$;

alter procedure public.insert_car(varchar, varchar, integer, varchar, varchar, integer) owner to postgres;
