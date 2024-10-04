create or replace function public.insert_car(
    IN model character varying,
    IN brand character varying,
    IN kilometers integer,
    IN price character varying,
    IN description character varying default null,
    IN year integer default null
) returns setof cars
language SQL
AS $$
  INSERT INTO cars(model, brand, kilometers, price, description, year)
    values (model, brand, kilometers, price, description, year) returning *;
$$;

alter function public.insert_car(varchar, varchar, integer, varchar, varchar, integer) owner to postgres;




CREATE OR REPLACE FUNCTION public.update_car(
    IN id INTEGER,
    IN model CHARACTER VARYING,
    IN brand CHARACTER VARYINg,
    IN kilometers INTEGER,
    IN price CHARACTER VARYING,
    IN description CHARACTER VARYING DEFAULT NULL,
    IN year INTEGER DEFAULT NULL
) RETURNS SETOF cars
LANGUAGE SQL
AS $$
  UPDATE cars SET model=update_car.model, brand=update_car.brand, 
      kilometers=update_car.kilometers, price=update_car.price,
      description=update_car.description, year=update_car.year
    WHERE id = update_car.id RETURNING *;
$$;

ALTER FUNCTION public.update_car(INTEGER, VARCHAR, VARCHAR, INTEGER, VARCHAR, VARCHAR, INTEGER) OWNER TO postgres;



CREATE OR REPLACE FUNCTION public.delete_car(IN id INTEGER) RETURNS SETOF cars
LANGUAGE SQL
AS $$
  DELETE FROM cars WHERE id = delete_car.id RETURNING *;
$$;

ALTER FUNCTION public.delete_car(INTEGER) OWNER TO postgres;


CREATE OR REPLACE FUNCTION public.get_cars_count()
RETURNS INTEGER AS $$
SELECT COUNT(*) FROM cars;
$$ LANGUAGE SQL IMMUTABLE;

ALTER FUNCTION public.get_cars_count() OWNER TO postgres;


CREATE OR REPLACE FUNCTION public.get_next_carid()
RETURNS INTEGER AS $$
SELECT last_value+1 FROM public.cars_id_seq;
$$ LANGUAGE SQL IMMUTABLE;

ALTER FUNCTION public.get_next_carid() OWNER TO postgres;