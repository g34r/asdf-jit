#include <iostream>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

static llvm::LLVMContext s_Context;
static llvm::IRBuilder<> s_Builder(s_Context);
static std::unique_ptr<llvm::Module> s_Module;

int main()
{
	std::cout << "[asdf JIT compiler]\n";
	std::cout << "Note that log below is not actually happening here; it's status of code generating for those behaviors.\n";

	std::clog << "[LOG] Making module...\n";
	s_Module = llvm::make_unique<llvm::Module>("top", s_Context);
	
	std::clog << "[LOG] Defining main entry func...\n";
	llvm::Function* main_func = llvm::Function::Create
		(
			llvm::FunctionType::get(llvm::Type::getInt32Ty(s_Context), false),
			llvm::Function::ExternalLinkage, 
			"main",
			s_Module.get()
		);

	std::clog << "[LOG] Setting insert point...\n";
	s_Builder.SetInsertPoint(llvm::BasicBlock::Create(s_Context, "", main_func));

	constexpr std::size_t arr_size = 3000;

	std::clog << "[LOG] Declaring data and pointer...\n";
	llvm::Value* data = s_Builder.CreateAlloca(s_Builder.getInt8PtrTy(), nullptr, "data");
	llvm::Value* ptr = s_Builder.CreateAlloca(s_Builder.getInt8PtrTy(), nullptr, "ptr");

	std::clog << "[LOG] Getting standard calloc function...\n";
	llvm::Function* calloc_func = llvm::cast<llvm::Function>
		(
		 s_Module->getOrInsertFunction("calloc", s_Builder.getInt8PtrTy(), s_Builder.getInt64Ty(), s_Builder.getInt64Ty(), nullptr)
		);

	std::clog << "[LOG] Allocating data pointer...(with calloc)\n";
	llvm::Value* data_ptr = s_Builder.CreateCall(calloc_func, {s_Builder.getInt64(arr_size), s_Builder.getInt64(1)});

	std::clog << "[LOG] Storing to data and pointer...\n";
	s_Builder.CreateStore(data_ptr, data);
	s_Builder.CreateStore(data_ptr, ptr);

	std::clog << "[LOG] Finished\n";
	return 0;
}
