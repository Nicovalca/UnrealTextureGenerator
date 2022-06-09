# Texture generator from xml data
## _Unreal Engine 4_

A Factory to be used within Unreal Engine 4 that allow the user to create textures from an XML file.

> This script is written following the AIV - Accademia Italiana Videogiochi course and its only purpose is 
> to experiment with the editor by following the teacher's lessons

## What is the result

From a file like this:

```xml
<texture size="64">
	<layers>
		<layer type="PerlinNoise">			
			<frequency_x>0.2</frequency_x>
			<frequency_y>0.2</frequency_y>
			<offset_x>0</offset_x>
			<offset_y>0</offset_y>
			<color>
				<R>255</R>
				<G>255</G>
				<B>255</B>
				<A>255</A>
			</color>
		</layer>
	</layers>
</texture>
```
the script will parse this data into a structure,
and then it generates a 64x64 pixels,
applying a Perlin Noise 

![alt text](https://cloudse1.seupload.com/cache/plugins/filepreviewer/51405/62c72fac155332119ddaef84a23f63f71e2c1e83619ea3e5b87d5ca70b70018d/1100x800_cropped.jpg)
