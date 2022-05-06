import os


def LoadClass(dir):

    for fileName in os.listdir(baseDir):
        fullPath = baseDir + fileName
        if os.path.splitext(fullPath)[1] in (".h", ".txt"):
            with open(fullPath, "r+") as file_object:
                if (file_object.name.__contains__("admf")):
                    ChangeClassName(file_object, fullPath)

    return


def ChangeClassName(file_object, fullPath):
    str = ""
    print(file_object.name)
    str = file_object.read()
    str = str.replace("String_", "String")
    str = str.replace("Vec2_", "Vec2")
    str = str.replace("ColorRGB_", "ColorRGB")
    str = str.replace("BinaryData_", "BinaryData")
    str = str.replace("Texture_", "Texture")
    str = str.replace("Array_", "Array")
    str = str.replace("BaseColorData_", "BaseColorData")
    str = str.replace("BaseColorDataSolid_", "BaseColorDataSolid")
    str = str.replace("BaseColorDataSolidBlock_", "BaseColorDataSolidBlock")
    str = str.replace("BaseColorDataMulti_", "BaseColorDataMulti")
    str = str.replace("Custom_", "Custom")
    str = str.replace("Geometry_", "Geometry")
    str = str.replace("ADMF_", "ADMF")
    str = str.replace("Material_", "Material")
    str = str.replace("MaterialDevice_", "MaterialDevice")
    str = str.replace("MaterialMetaData_", "MaterialMetaData")
    str = str.replace("MaterialLayer_", "MaterialLayer")
    str = str.replace("LayerSpec_", "LayerSpec")
    str = str.replace("LayerRefraction_", "LayerRefraction")
    str = str.replace("LayerBasic_", "LayerBasic")
    str = str.replace("LayerTransform_", "LayerTransform")
    str = str.replace("PhysicsComposition_", "PhysicsComposition")
    str = str.replace("Physics_", "Physics")
    str = str.replace("BaseColor_", "BaseColor")
    str = str.replace("Normal_", "Normal")
    str = str.replace("Alpha_", "Alpha")
    str = str.replace("Metalness_", "Metalness")
    str = str.replace("Roughness_", "Roughness")
    str = str.replace("Specular_", "Specular")
    print(str)
    with open(fullPath, "w+") as file_new:
        file_new.write(str)
    return


################################################################################
if __name__ == "__main__":
    baseDir = "D:\\WorkDisk\\WorkPro\\DoxygenDemo\\DoxygenDemo\\"
    LoadClass(baseDir)
