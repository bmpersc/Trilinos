#include <gtest/gtest.h>                // for AssertHelper, EXPECT_EQ, etc
#include <stk_io/StkMeshIoBroker.hpp>   // for StkMeshIoBroker
#include <stk_io/FillMesh.hpp>
#include <stk_unit_test_utils/MeshFixture.hpp>

namespace
{

class AttributesInFile : public stk::unit_test_util::MeshFixture {};

TEST_F(AttributesInFile, reading_fieldsReturnedInOrder)
{
    if(stk::parallel_machine_size(get_comm()) == 1)
    {
        setup_empty_mesh(stk::mesh::BulkData::AUTO_AURA);

        const std::string filename = "fileWithAttr.e";
        stk::io::StkMeshIoBroker stkIo(get_comm());
        stk::io::fill_mesh_preexisting(stkIo, filename, get_bulk());

        const stk::mesh::Part *blockPart = get_meta().get_part("block_10");
        ASSERT_NE(nullptr, blockPart);
        stk::mesh::FieldVector attrFields = stkIo.get_ordered_attribute_fields(blockPart);
        std::vector<std::string> expectedNames = {"area", "i1", "i2", "j", "orient", "offset"};
        ASSERT_EQ(expectedNames.size(), attrFields.size());
        for(size_t i=0; i<expectedNames.size(); i++)
            EXPECT_EQ(expectedNames[i], attrFields[i]->name());
    }
}

}
